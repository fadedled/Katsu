
#include <katsu/kt.h>
#include "../../joypad_common.h"


u32  __kt_JoypadInit(void)
{
	joy_state[0].active = 1;

	return KT_OK;
}


void __kt_JoypadExit(void)
{

}


void __kt_JoypadAttrSet(u32 attr, u32 val)
{

}


void __kt_JoypadPoll(void)
{
	//Update the prev values per active joypad
	for (u32 i = 0; i < MAX_JOYPADS; ++i) {
		if (joy_state[i].active) {
			joy_state[i].btn_prev = joy_state[i].btn;
			joy_state[i].stick_prev[0] = joy_state[i].stick[0];
			joy_state[i].stick_prev[1] = joy_state[i].stick[1];
			joy_state[i].stick_prev[2] = joy_state[i].stick[2];
			joy_state[i].stick_prev[3] = joy_state[i].stick[3];
		}
	}
}
