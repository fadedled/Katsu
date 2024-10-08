



#include <katsu/input.h>

#define MAX_EVENT_BUFFER_SIZE		(1024)


struct {
	u32 kb_head;
	u32 kb_num;
} input_state;

//Circular event buffers
KeyEvent kb_ev_buff[MAX_EVENT_BUFFER_SIZE];


void __kt_KeyboardInit(void)
{
	input_state.kb_head = 0;
	input_state.kb_num = 0;
}

u32 __kt_KeyboardAddEvent(KeyEvent *kev)
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


u32 kt_KeyboardGetEvent(KeyEvent *kev)
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
