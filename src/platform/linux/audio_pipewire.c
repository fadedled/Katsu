
#include <katsu/kt.h>
#include "../../audio_common.h"
#include <pipewire/pipewire.h>
#include <math.h>
#include <spa/param/audio/format-utils.h>




int dummy_argc = 1;
char *dummy_argv[] = {"Katsu Application"};


struct pw_context *ctx;
struct pw_core *core;
struct pw_registry *registry;
struct spa_hook registry_listener;


struct data_t {
	struct pw_thread_loop *loop;
	struct pw_stream *stream;
} data;


/* [__kt_AudioCallback] */
static void __kt_AudioCallback(void *userdata)
{
	struct pw_buffer *b;
	struct spa_buffer *buf;
	u32 n_frames, stride;
	s16 *dst;

	if ((b = pw_stream_dequeue_buffer(data.stream)) == NULL) {
		pw_log_warn("out of buffers: %m");
		return;
	}

	buf = b->buffer;
	if ((dst = buf->datas[0].data) == NULL)
		return;

	stride = sizeof(s16) * KT_AUDIO_CHANNELS;
	n_frames = buf->datas[0].maxsize / stride;
	if (b->requested)
		n_frames = SPA_MIN(b->requested, n_frames);

	if (audio_state.user_callback) {
		audio_state.user_callback(dst, n_frames, audio_state.user_data);
	}


	buf->datas[0].chunk->offset = 0;
	buf->datas[0].chunk->stride = stride;
	buf->datas[0].chunk->size = n_frames * stride;

	pw_stream_queue_buffer(data.stream, b);
}
/* [__kt_AudioCallback] */


static const struct pw_stream_events stream_events = {
	PW_VERSION_STREAM_EVENTS,
	.process = __kt_AudioCallback,
};


u32  __kt_AudioInit(void)
{
	const struct spa_pod *params[1];
	u8 buffer[1024];
	struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

	pw_init(&dummy_argc, (char ***) &dummy_argv);

	data.loop = pw_thread_loop_new("kt-audio", NULL);
	data.stream = pw_stream_new_simple(
			pw_thread_loop_get_loop(data.loop),
			"kt-stream-audio",
			pw_properties_new(
				PW_KEY_MEDIA_TYPE, "Audio",
				PW_KEY_MEDIA_CATEGORY, "Playback",
				PW_KEY_MEDIA_ROLE, "Game",
				NULL),
			&stream_events,
			NULL);

	params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat,
											&SPA_AUDIO_INFO_RAW_INIT(
											.format = SPA_AUDIO_FORMAT_S16,
											.channels = KT_AUDIO_CHANNELS,
											.rate = KT_AUDIO_SAMPLE_RATE ));

	u32 con_err = 0;
	con_err = pw_stream_connect(data.stream,
					PW_DIRECTION_OUTPUT,
					PW_ID_ANY,
					PW_STREAM_FLAG_AUTOCONNECT |
					PW_STREAM_FLAG_MAP_BUFFERS |
					PW_STREAM_FLAG_RT_PROCESS,
					params, 1);
	if (con_err != 0) {
		return KT_ERROR_NO_AUDIO;
	}

	if (pw_thread_loop_start(data.loop)) {
		return KT_ERROR_NO_AUDIO;
	}

	return KT_OK;
}


void __kt_AudioExit(void)
{
	pw_thread_loop_stop(data.loop);
	pw_stream_destroy(data.stream);
	pw_thread_loop_destroy(data.loop);
	pw_deinit();
}


void __kt_AudioAttrSet(u32 attr, u32 val)
{

}
