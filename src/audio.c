
#include <katsu/kt.h>
#include "audio_common.h"

AudioState audio_state;

void kt_AudioSetCallback(KTAudioFunc func, void *user_data)
{
	audio_state.user_callback = func;
	audio_state.user_data = user_data;
}
