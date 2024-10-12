



#include <katsu/input.h>
#include "input_common.h"
#include "video_common.h"


//Circular event buffer
KTKeyEvent kb_ev_buff[KT_MAX_EVENT_BUFFER_SIZE];
InputState input_state;

void __kt_InputInit(void)
{
	//At startup no polling is done for keyboard or mouse
	input_state.polling = 0;
	input_state.kb_head = 0;
	input_state.kb_num = 0;
	input_state.mouse.x = 0;
	input_state.mouse.y = 0;
	input_state.mouse.dx = 0;
	input_state.mouse.dy = 0;
	input_state.mouse.btn0 = 0;
	input_state.mouse.btn1 = 0;
	input_state.mouse.scroll = 0;
}

void kt_KeyboardEnablePolling(u32 enable)
{
	input_state.polling = (input_state.polling & ~KT_KEYBOARD_POLLING) | (enable ? KT_KEYBOARD_POLLING : 0);
}

u32 __kt_KeyboardAddEvent(KTKeyEvent *kev)
{
	u32 tail = (input_state.kb_head + input_state.kb_num) & (KT_MAX_EVENT_BUFFER_SIZE - 1);
	if (input_state.kb_num < KT_MAX_EVENT_BUFFER_SIZE) {
		kb_ev_buff[tail] = *kev;
		input_state.kb_num++;
		return 1;
	} else {	//Buffer is full, will eat head of buffer.
		kb_ev_buff[tail] = *kev;
		input_state.kb_head = (input_state.kb_head + 1) & (KT_MAX_EVENT_BUFFER_SIZE - 1);
		return 0;
	}
}


u32 kt_KeyboardGetEvent(KTKeyEvent *kev)
{
	if (input_state.kb_num) {
		u32 head = input_state.kb_head;
		*kev = kb_ev_buff[head++];
		input_state.kb_head = head & (KT_MAX_EVENT_BUFFER_SIZE - 1);
		input_state.kb_num--;
		return 1;
	}
	return 0;
}


u32 kt_KeyboardFlushEvents(void)
{
	u32 ev_flushed = input_state.kb_num;
	input_state.kb_num = 0;
	return ev_flushed;
}


void kt_MouseEnablePolling(u32 enable)
{
	input_state.polling = (input_state.polling & ~KT_MOUSE_POLLING) | (enable ? KT_MOUSE_POLLING : 0);
}

void __kt_MouseUpdateState(s32 x, s32 y)
{
	u32 mouse_prev_x = input_state.mouse.x;
	u32 mouse_prev_y = input_state.mouse.y;
	f32 pix_w = (f32) (x - (s32) vstate.frame_output_x) * (f32) vstate.output_w;
	f32 pix_h = (f32) (y - (s32) vstate.frame_output_y) * (f32) vstate.output_h;
	input_state.mouse.x = (s32) (vstate.frame_output_w > 0.0f ? pix_w / vstate.frame_output_w : 0);
	input_state.mouse.y = (s32) (vstate.frame_output_h > 0.0f ? pix_h / vstate.frame_output_h : 0);
	input_state.mouse.dx = input_state.mouse.x - mouse_prev_x;
	input_state.mouse.dy = input_state.mouse.y - mouse_prev_y;
}


u32 kt_MouseGetState(KTMouse *mouse)
{
	*mouse = input_state.mouse;
	return 0;
}
