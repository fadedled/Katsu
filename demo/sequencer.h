/*
 * step_sequencer.h
 */


#ifndef __STEP_SEQUENCER_H__
#define __STEP_SEQUENCER_H__

#include <katsu/kt.h>



void sseq_Open(u32 max_voices, f32 volume);
void sseq_Close(void);


void sseq_SetSpeed(f32 speed);
void sseq_SetVolume(f32 volume);
f32  sseq_GetSpeed(void);
f32  sseq_GetVolume(void);
u32 sseq_SetButton(u32 voice, u32 entrie, u32 state);

#endif /*__STEP_SEQUENCER_H__*/
