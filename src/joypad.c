



#include <katsu/joypad.h>
#include "joypad_common.h"


JoypadState joy_state[MAX_JOYPADS];


u32 kt_JoyIsActive(u32 joy_id)
{
	joy_id &= MAX_JOYPADS-1;
	return joy_state[joy_id].active;
}


u32 kt_JoyButtonHeld(u32 joy_id)
{
	joy_id &= MAX_JOYPADS-1;
	return joy_state[joy_id].btn;
}


u32 kt_JoyButtonUp(u32 joy_id)
{
	joy_id &= MAX_JOYPADS-1;
	return joy_state[joy_id].btn_prev & ~joy_state[joy_id].btn;
}


u32 kt_JoyButtonDown(u32 joy_id)
{
	joy_id &= MAX_JOYPADS-1;
	return ~joy_state[joy_id].btn_prev & joy_state[joy_id].btn;
}


s8 kt_JoyStick(u32 joy_id, u32 stick_id)
{
	joy_id &= MAX_JOYPADS-1;
	return joy_state[joy_id].stick[stick_id & 0x3];
}
