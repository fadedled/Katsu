


#include <xinput.h>
#include <stdio.h>
#include <katsu/kt.h>
#include "../../joypad_common.h"


#define MAX_WIN_JOYPADS			4		//Max joypads for Windows (up to 4)
#define AXIS_CODE_MAX			8

/*Joystick state for a max of 8 joypads*/
static struct Joypad_Device_tag {
	/* Platform dependant state */
	int				fd;				//File descriptor
	u64				id;				//device ID
} joy_dev[MAX_WIN_JOYPADS];


//joypad assigned to the keyboard
u32 kb_joy = 0;


static void __kt_JoyClose(u32 joy_id)
{
	joy_state[joy_id].active = 0;
	joy_state[joy_id].btn = 0;
	joy_state[joy_id].stick[JOY_STICK_LX] = 0;
	joy_state[joy_id].stick[JOY_STICK_LY] = 0;
	joy_state[joy_id].stick[JOY_STICK_RX] = 0;
	joy_state[joy_id].stick[JOY_STICK_RY] = 0;

	/*Find next joypad for keyboard */
	joy_state[kb_joy].active = 0u;
	for (u32 k = 0; k < MAX_WIN_JOYPADS; ++k) {
		if (joy_dev[k].fd == -1) {
			kb_joy = k;
			joy_state[kb_joy].active = 1u;
			return;
		}
	}
}


void __kt_JoypadSearchAndOpen(void)
{
	for (u32 i = 0; i < MAX_WIN_JOYPADS; ++i) {
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			joy_state[i].active = 1;
		}
	}
	//TODO: do keyboard search
	/*Set the keyboard buttons for the remaning controller*/
}


u32  __kt_JoypadInit(void)
{
	//Get all currently conected gamepads
	__kt_JoypadSearchAndOpen();

	//Set the keyboard as joypad0
	kb_joy = 0;
	joy_state[0].active = 1u;

	return KT_OK;
}


void __kt_JoypadExit(void)
{
	for (int i = 0; i < MAX_WIN_JOYPADS; ++i) {
		__kt_JoyClose(i);
	}
}


void __kt_JoypadAttrSet(u32 attr, u32 val)
{

}

const u32 xinput_gpad_btn_converter[16] = {
	JOY_UP,		//XINPUT_GAMEPAD_DPAD_UP 			0x0001
	JOY_DOWN,	//XINPUT_GAMEPAD_DPAD_DOWN 			0x0002
	JOY_LEFT,	//XINPUT_GAMEPAD_DPAD_LEFT 			0x0004
	JOY_RIGHT,	//XINPUT_GAMEPAD_DPAD_RIGHT 		0x0008
	JOY_STR,	//XINPUT_GAMEPAD_START 				0x0010
	JOY_SEL,	//XINPUT_GAMEPAD_BACK 				0x0020
	0x0, 		//XINPUT_GAMEPAD_LEFT_THUMB 		0x0040
	0x0,		//XINPUT_GAMEPAD_RIGHT_THUMB 		0x0080
	JOY_L,		//XINPUT_GAMEPAD_LEFT_SHOULDER 		0x0100
	JOY_R,		//XINPUT_GAMEPAD_RIGHT_SHOULDER 	0x0200
	//TODO: Should rename to avoid dumb diferences between controllers
	JOY_A,		//XINPUT_GAMEPAD_A 					0x1000
	JOY_B,		//XINPUT_GAMEPAD_B 					0x2000
	JOY_X,		//XINPUT_GAMEPAD_X 					0x4000
	JOY_Y		//XINPUT_GAMEPAD_Y					0x8000
};

void __kt_JoypadPoll(void)
{
	XINPUT_STATE state = {};

	//Update the prev values per active joypad
	for (u32 i = 0; i < MAX_WIN_JOYPADS; ++i) {
		if (joy_state[i].active) {
			if (XInputGetState(i, &state) == ERROR_SUCCESS) {
				joy_state[i].btn_prev = joy_state[i].btn;
				joy_state[i].stick_prev[0] = joy_state[i].stick[0];
				joy_state[i].stick_prev[1] = joy_state[i].stick[1];
				joy_state[i].stick_prev[2] = joy_state[i].stick[2];
				joy_state[i].stick_prev[3] = joy_state[i].stick[3];

				XINPUT_GAMEPAD xgamepad = state.Gamepad;
				u32 btn = 0;
				for (k = 0; k < 16; ++k) {
					btn |= xinput_gpad_btn_converter[k] * (xgamepad.wButtons >> k) & 1;
				}
				btn |= (xgamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * JOY_TL;
				btn |= (xgamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * JOY_TR;
				joy_state[i].btn = btn;
				joy_state[i].stick[JOY_STICK_LX] = (s8)(((xgamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE |
				xgamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * xgamepad.sThumbLX) >> 8);
				joy_state[i].stick[JOY_STICK_LY] = (s8)(((xgamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE |
				xgamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * xgamepad.sThumbLY) >> 8);
				joy_state[i].stick[JOY_STICK_RX] = (s8)(((xgamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE |
				xgamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * xgamepad.sThumbRX) >> 8);
				joy_state[i].stick[JOY_STICK_RY] = (s8)(((xgamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE |
				xgamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * xgamepad.sThumbRY) >> 8);
			}
			else {
				/*Deactivate Joypad if there are errors while reading */
				__kt_JoyClose(i);
			}
		}
	}

	/*Set the keyboard buttons for the remaning controller*/
	//if (joy_dev[kb_joy].fd == -1) {
	//	joy_state[kb_joy].btn = kb_btn;
	//}
}
