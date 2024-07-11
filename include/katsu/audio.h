/*
 * audio.h
 */


#ifndef __KT_AUDIO_H__
#define __KT_AUDIO_H__

#include <katsu/types.h>

#define KT_AUDIO_SAMPLE_RATE       48000
#define KT_AUDIO_SAMPLE_BUFFER     2048
#define KT_AUDIO_CHANNELS          2


typedef void (*KTAudioFunc)(s16 *stream, u32 length, void *user_data);

void kt_AudioSetCallback(KTAudioFunc func, void *user_data);


#endif /*__KT_AUDIO_H__*/
