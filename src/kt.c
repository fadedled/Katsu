



#include <katsu/kt.h>
#include <stdlib.h>

#include "opengl/gfx_ogl.h"
#include "video_common.h"
#include "joypad_common.h"
#include "audio_common.h"



static KTExitFunc kt_exit_func = 0x0;

//Inits katsu
u32 kt_Init(void)
{
	u32 err = KT_OK;

	kt_VideoOutputSet(VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);
	kt_VideoFillModeSet(VIDEO_FILL_OUT_SCALE);
	kt_VideoFilterSet(VIDEO_FILTER_SHARP);
	//kt_VideoFrameSet(VIDEO_FRAME_2X);
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


//Polls joypads and window events
void kt_Poll(void)
{
	__kt_JoypadPoll();
	__kt_VideoPoll();
}


//Draws the screen
void kt_Draw(void)
{
	ogl_Draw();
	__kt_VideoSwapBuffers();
}


