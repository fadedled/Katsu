/*
 * audio.h
 */


#ifndef __KT_AUDIO_H__
#define __KT_AUDIO_H__

/*! \file audio.h
 \ brief Audio subsystem functions

 */

#include <katsu/types.h>

#define KT_AUDIO_SAMPLE_RATE       48000
#define KT_AUDIO_SAMPLE_BUFFER     2048
#define KT_AUDIO_CHANNELS          2

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*! \typedef void (*KTAudioFunc)(s16 *stream, u32 length, void *user_data)
 * \brief function pointer typedef for audio callback.
 * \param[in] stream Array to be filled with 16bit signed data, it's size is equal to (length * KT_AUDIO_CHANNELS).
 * \param[in] length Number of samples that must be filled.
 * \param[in] user_data Pointer to user data block that was passed to kt_AudioSetCallback().
 */
typedef void (*KTAudioFunc)(s16 *stream, u32 length, void *user_data);

/*!
 * \fn void kt_AudioSetCallback(KTAudioFunc func, void *user_data)
 * \brief Sets the audio callback function.
 *
 * \details After this function is called, the audio subsystem will call func
 * when audio data is needed. Since the audio subsystem is in another thread,
 * it is best to pass only one function that will take care of all audio related
 * work and sync to that function from the main application thread.
 *
 * \param[in] func Callback function fo.
 * \param[in] user_data Pointer to data block that will be passed to func.
 *
 * \return none
 */
void kt_AudioSetCallback(KTAudioFunc func, void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_AUDIO_H__*/
