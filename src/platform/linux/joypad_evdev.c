

#define _GNU_SOURCE

#include <linux/input.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <katsu/kt.h>
#include "../../joypad_common.h"


/*Defines for joystick*/
#define BITS_PER_LONG			(sizeof(long) * 8)
#define NBITS(x)				((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)					((x)%BITS_PER_LONG)
#define BIT(x)					(1UL<<OFF(x))
#define LONG(x)					((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)


#define AXIS_CODE_MAX			8


typedef struct BtnCode_tag {
	u32				code;		//Device Button code
	u32				value;		//value for joypad
} BtnCode;

typedef struct AxisCode_tag {
	u32				code;		//Device axis code
	s32				min;		//Sticks min value
	s32				max;		//Sticks max value
	s32				value;		//Sticks current value
	s32				deadzone;	//Sticks deadzone
} AxisCode;

/*Joystick state for a max of 8 joypads*/
static struct Joypad_Device_tag {
	/* Platform dependant state */
	int				fd;				//File descriptor
	u64				id;				//device ID
	dev_t			rdev;			//device ID

	u32				btn_code[12];	//joypad button codes
	AxisCode		axis_code[AXIS_CODE_MAX];	//joypad axis codes
} joy_dev[MAX_JOYPADS];


//joypad assigned to the keyboard
u32 kb_joy = 0;



static int __isEvdevJoystick(const struct dirent *dir)
{
	return !strncmp("event", dir->d_name, 5);
}


static void __kt_JoyClose(u32 joy_id)
{
	if (joy_dev[joy_id].fd >= 0) {
		close(joy_dev[joy_id].fd);
	}
	joy_dev[joy_id].fd = -1;
	joy_dev[joy_id].id = 0;
	joy_dev[joy_id].rdev = -1;

	joy_state[joy_id].active = 0;
	joy_state[joy_id].btn = 0;
	joy_state[joy_id].stick[JOY_STICK_LX] = 0;
	joy_state[joy_id].stick[JOY_STICK_LY] = 0;
	joy_state[joy_id].stick[JOY_STICK_RX] = 0;
	joy_state[joy_id].stick[JOY_STICK_RY] = 0;

	/*Find next joypad for keyboard */
	joy_state[kb_joy].active = 0u;
	for (u32 k = 0; k < MAX_JOYPADS; ++k) {
		if (joy_dev[k].fd == -1) {
			kb_joy = k;
			joy_state[kb_joy].active = 1u;
			return;
		}
	}
}

time_t last_mtime;

void __kt_JoypadSearchAndOpen(void)
{
	/* Get Controller inputs */
	char filename[256];
	struct stat sb;
	struct dirent **nlist;
	u64 id;

	/* We have no more joypad slots to fill */
	if (joy_dev[kb_joy].fd != -1) {
		return;
	}
	/* Check if files in input dir have changed */
	stat("/dev/input", &sb);
	if (last_mtime == sb.st_mtim.tv_sec) {
		return;
	}
	last_mtime = sb.st_mtim.tv_sec;

	//Attempt to find a new joystick
	int ndev = scandir("/dev/input", &nlist, __isEvdevJoystick, alphasort);
	for (u32 i = 0; i < ndev; ++i) {
		//get info on file
		sprintf(filename, "/dev/input/%.64s", nlist[i]->d_name);
		//See if we can open the file
		int fd = open((char *) filename, O_RDONLY | O_NONBLOCK);
		if (fd < 0) {
			goto no_dev;
		}
		//See if device id is already in use
		stat(filename, &sb);
		for (u32 j = 0; j < MAX_JOYPADS; ++j) {
			if (joy_dev[j].rdev == sb.st_rdev) {
				goto no_dev;
			}
		}

		/*Open the device that's not in use*/
		u64 bit[EV_MAX][NBITS(KEY_MAX)] = {0};
		s32 abs[6] = {0};
		u32 type = EV_ABS;
		u32 ax_code[AXIS_CODE_MAX] = {ABS_HAT0X, ABS_HAT0Y,
										ABS_X, ABS_Y, ABS_RX, ABS_RY,
										ABS_Z, ABS_RZ};
		/*Set the ABS values*/
		//TODO: Make the code assigment more straightforward
		ioctl(fd, EVIOCGID, &id);
		ioctl(fd, EVIOCGBIT(type, KEY_MAX), bit[type]);
		for (u32 j = 0; j < AXIS_CODE_MAX; ++j) {
			u32 code = ax_code[j];
			if (test_bit(code, bit[type])) {
				ioctl(fd, EVIOCGABS(code), abs);
				joy_dev[kb_joy].axis_code[j].code = code;
				joy_dev[kb_joy].axis_code[j].min = abs[1];
				joy_dev[kb_joy].axis_code[j].max = abs[2];
				joy_dev[kb_joy].axis_code[j].value = abs[0];
				joy_dev[kb_joy].axis_code[j].deadzone = abs[4];
			}
		}
		joy_dev[kb_joy].fd = fd;
		joy_dev[kb_joy].id = id;
		joy_dev[kb_joy].rdev = sb.st_rdev;

		/*Find next joypad for keyboard */
		for (u32 k = 0; k < MAX_JOYPADS; ++k) {
			if (joy_dev[k].fd == -1) {
				kb_joy = k;
				joy_state[kb_joy].active = 1u;
				break;
			}
		}
no_dev:
	}
}


u32  __kt_JoypadInit(void)
{
	u32 i = 0;
	for (; i < MAX_JOYPADS; ++i) {
		joy_state[i].active = 0;
		joy_state[i].btn = 0;
		joy_state[i].stick[0] = 0;
		joy_state[i].stick[1] = 0;
		joy_state[i].stick[2] = 0;
		joy_state[i].stick[3] = 0;

		joy_dev[i].fd = -1;
		joy_dev[i].id = 0;
		joy_dev[i].rdev = -1;

		/* Standard linux button mapping */
		joy_dev[i].btn_code[0] = BTN_SOUTH;
		joy_dev[i].btn_code[1] = BTN_EAST;
		joy_dev[i].btn_code[2] = BTN_WEST;
		joy_dev[i].btn_code[3] = BTN_NORTH;
		joy_dev[i].btn_code[4] = BTN_TL;
		joy_dev[i].btn_code[5] = BTN_TR;
		joy_dev[i].btn_code[6] = BTN_TL2;
		joy_dev[i].btn_code[7] = BTN_TR2;
		joy_dev[i].btn_code[8] = BTN_START;
		joy_dev[i].btn_code[9] = BTN_SELECT;

		joy_dev[i].axis_code[0].code = ABS_HAT0X;
		joy_dev[i].axis_code[1].code = ABS_HAT0Y;
		joy_dev[i].axis_code[2].code = ABS_X;
		joy_dev[i].axis_code[3].code = ABS_Y;
		joy_dev[i].axis_code[4].code = ABS_RX;
		joy_dev[i].axis_code[5].code = ABS_RY;
		joy_dev[i].axis_code[6].code = ABS_Z;
		joy_dev[i].axis_code[7].code = ABS_RZ;
	}

	//Set the keyboard as joypad0
	kb_joy = 0;
	joy_state[0].active = 1u;

	return KT_OK;
}


void __kt_JoypadExit(void)
{
	for (int i = 0; i < MAX_JOYPADS; ++i) {
		__kt_JoyClose(i);
	}
}


void __kt_JoypadAttrSet(u32 attr, u32 val)
{

}


void __kt_JoypadPoll(void)
{
	struct input_event ev[32];
	s32 j, k, len, code, value;

	//Update the prev values per active joypad
	for (u32 i = 0; i < MAX_JOYPADS; ++i) {
		if (joy_state[i].active) {
			joy_state[i].btn_prev = joy_state[i].btn;
			joy_state[i].stick_prev[0] = joy_state[i].stick[0];
			joy_state[i].stick_prev[1] = joy_state[i].stick[1];
			joy_state[i].stick_prev[2] = joy_state[i].stick[2];
			joy_state[i].stick_prev[3] = joy_state[i].stick[3];

			if (joy_dev[i].fd != -1) {
				u32 btn = joy_state[i].btn;
				errno = 0;
				//TODO: make this better
				while ((len = read(joy_dev[i].fd, ev, sizeof(ev))) > 0) {
					len /= sizeof(*ev);
					for (j = 0; j < len; ++j) {
						code = ev[j].code;
						value = ev[j].value;
						switch(ev[j].type) {
							case EV_KEY: {
								for (k = 0; k < 10; ++k) {
									if (joy_dev[i].btn_code[k] == code) {
										k += 4;
										btn = (btn & ~(1 << k)) | ((value != 0) << k);
										break;
									}
								}
							} break;
							case EV_ABS: {
								for (k = 0; k < AXIS_CODE_MAX; ++k) {
									if (joy_dev[i].axis_code[k].code == code) {
										f32 range = joy_dev[i].axis_code[k].max - joy_dev[i].axis_code[k].min;
										s8 sval = (s8) ((((f32) value - joy_dev[i].axis_code[k].min) / range) * 255.0) - 128.0;
										switch (code) {
											case ABS_HAT0Y: {
												u32 val = (sval > 16 ? 2u : (sval < -16));
												btn = (btn & (~(3u << 2))) | (val << 2);
												printf("Y sval : %d\n", sval);
												printf("%d\n", btn);
											} break;
											case ABS_HAT0X: {
												u32 val = (sval > 16 ? 2u : (sval < -16));
												btn = (btn & (~3u)) | val;
											} break;
											case ABS_X: {
												joy_state[i].stick[JOY_STICK_LX] = sval;
											} break;
											case ABS_Y: {
												joy_state[i].stick[JOY_STICK_LY] = sval;
											} break;
											case ABS_Z: {
												//TODO: TL
											} break;
											case ABS_RX: {
												joy_state[i].stick[JOY_STICK_RX] = sval;
											} break;
											case ABS_RY: {
												joy_state[i].stick[JOY_STICK_RY] = sval;
											} break;
											case ABS_RZ: {
												//TODO: TR
											} break;
										}
									}
								}
							} break;
											default: break;
						}
					}
				}
				joy_state[i].btn = btn;

				/*Deactivate Joypad if there are errors while reading */
				if (errno == ENODEV) {
					__kt_JoyClose(i);
				}
			}
		}
	}

	/*Set the keyboard buttons for the remaning controller*/
	//if (joy_dev[kb_joy].fd == -1) {
	//	joy_state[kb_joy].btn = kb_btn;
	//}
}
