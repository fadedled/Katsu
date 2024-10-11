



#include <katsu/input.h>
#include "video_common.h"

#define MAX_EVENT_BUFFER_SIZE		(1024)


struct {
	u32 kb_head;
	u32 kb_num;
	KTMouse mouse;
} input_state;

//Circular event buffers
KTKeyEvent kb_ev_buff[MAX_EVENT_BUFFER_SIZE];


void __kt_KeyboardInit(void)
{
	input_state.kb_head = 0;
	input_state.kb_num = 0;
}

u32 __kt_KeyboardAddEvent(KTKeyEvent *kev)
{
	u32 tail = (input_state.kb_head + input_state.kb_num) & (MAX_EVENT_BUFFER_SIZE - 1);
	if (input_state.kb_num < MAX_EVENT_BUFFER_SIZE) {
		kb_ev_buff[tail] = *kev;
		input_state.kb_num++;
		return 1;
	} else {	//Buffer is full, will eat head of buffer.
		kb_ev_buff[tail] = *kev;
		input_state.kb_head = (input_state.kb_head + 1) & (MAX_EVENT_BUFFER_SIZE - 1);
		return 0;
	}
}


u32 kt_KeyboardGetEvent(KTKeyEvent *kev)
{
	if (input_state.kb_num) {
		u32 head = input_state.kb_head;
		*kev = kb_ev_buff[head++];
		input_state.kb_head = head & (MAX_EVENT_BUFFER_SIZE - 1);
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


void __kt_MouseSetState(KTMouse *m, s32 x, s32 y)
{
	input_state.mouse = *m;
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
