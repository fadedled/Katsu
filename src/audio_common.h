/*
 * audio_common.h
 */


#ifndef __KT_AUDIO_COMMON_H__
#define __KT_AUDIO_COMMON_H__

#include <katsu/audio.h>

typedef struct AudioState_t {
	KTAudioFunc user_callback;
	void* user_data;
} AudioState;

extern AudioState audio_state;

u32  __kt_AudioInit(void);
void __kt_AudioExit(void);
void __kt_AudioAttrSet(u32 attr, u32 val);

#endif /*__KT_AUDIO_COMMON_H__*/
