



#include <katsu/kt.h>
#include <stdlib.h>

#include "opengl/gfx_ogl.h"
#include "video_common.h"
#include "joypad_common.h"
#include "audio_common.h"



static KTExitFunc kt_exit_func = 0x0;

static u32 is_inited = 0;

//Inits katsu
u32 kt_Init(void)
{
	if (is_inited) {
		return -1;
	}
	u32 err = KT_OK;

	kt_VideoSetResolution(KT_VIDEO_RES_STD);
	kt_VideoOutputSet(KT_VIDEO_STD_WIDTH, KT_VIDEO_STD_HEIGHT);
	kt_VideoFillModeSet(KT_VIDEO_FILL_OUT_SCALE);
	kt_VideoFilterSet(KT_VIDEO_FILTER_SHARP);
	//kt_VideoFrameSet(KT_VIDEO_FRAME_2X);
	//kt_VideoTitleSet(const char* title);

	err = __kt_VideoInit();
	if (err != KT_OK) {
		return err;
	}
	err = __kt_JoypadInit();
	if (err != KT_OK) {
		goto err_joypad;
		return err;
	}
	err = __kt_AudioInit();
	if (err != KT_OK) {
		goto err_audio;
	}
	//Init the corresponding graphics API
	ogl_Init();
	is_inited = 1;
	return err;

err_audio:
	__kt_JoypadExit();
err_joypad:
	__kt_VideoExit();
	return err;
}

//Exits program
void kt_Exit(u32 exit_code)
{
	if (!is_inited) {
		return;
	}

	if (kt_exit_func) {
		if (kt_exit_func(exit_code) == KT_EXIT_CANCEL) {
			return;
		}
	}

	__kt_AudioExit();
	__kt_JoypadExit();
	__kt_VideoExit();
	exit(exit_code);
}

//Sets function to be called when trying to exit Katsu
void kt_ExitFuncSet(KTExitFunc func)
{
	kt_exit_func = func;
}

#define JOY_SEARCH_ALARM		60
u32 joy_search_timer = 0;

//Polls joypads and window events
void kt_Poll(void)
{
	//TODO: This should not be automatic...
	if (joy_search_timer == JOY_SEARCH_ALARM) {
		__kt_JoypadSearchAndOpen();
		joy_search_timer = 0;
	}
	__kt_JoypadPoll();
	__kt_VideoPoll();
	joy_search_timer++;
}


//Draws the screen
void kt_Draw(void)
{
	ogl_Draw();
	__kt_VideoSwapBuffers();
}


