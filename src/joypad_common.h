/*
 * joypad_common.h
 */


#ifndef __KT_JOYPAD_COMMON_H__
#define __KT_JOYPAD_COMMON_H__

typedef struct JoypadState_t {
	u32 active;

	u32 btn;
	u32 btn_prev;

	s8 stick[4];
	s8 stick_prev[4];

} JoypadState;

extern JoypadState joy_state[MAX_JOYPADS];

u32  __kt_JoypadInit(void);
void __kt_JoypadExit(void);
void __kt_JoypadAttrSet(u32 attr, u32 val);
void __kt_JoypadPoll(void);


#endif /*__KT_JOYPAD_COMMON_H__*/
