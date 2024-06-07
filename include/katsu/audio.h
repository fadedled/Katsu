/*
 * audio.h
 */


#ifndef __KT_AUDIO_H__
#define __KT_AUDIO_H__

#include <katsu/types.h>

#define KT_AUDIO_SAMPLE_RATE       48000
#define KT_AUDIO_SAMPLE_BUFFER     2048


typedef void (*KTAudioFunc)(u16 *stram, u32 length);

//XXX: void kt_AudioCallbackFunc(KTAudioFunc func);


#endif /*__KT_AUDIO_H__*/
