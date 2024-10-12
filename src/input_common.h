/*
 * input_common.h
 */


#ifndef __KT_INPUT_COMMON_H__
#define __KT_INPUT_COMMON_H__


#define KT_MAX_EVENT_BUFFER_SIZE        (1024)
#define KT_KEYBOARD_POLLING             1
#define KT_MOUSE_POLLING                2

typedef struct InputState_t {
	u32 polling;
	u32 kb_head;
	u32 kb_num;
	KTMouse mouse;
} InputState;

extern InputState input_state;

void __kt_InputInit(void);
u32 __kt_KeyboardAddEvent(KTKeyEvent *kev);
void __kt_MouseUpdateState(s32 new_x, s32 new_y);

#endif /*__KT_VIDEO_COMMON_H__*/
