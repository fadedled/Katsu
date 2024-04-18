
#include <katsu/kt.h>
#include "video_common.h"

VideoState vstate;


void kt_VideoOutputSet(u32 w, u32 h)
{
	w = MIN(w, VIDEO_MAX_WIDTH);
	h = MIN(h, VIDEO_MAX_HEIGHT);
	w = MAX(w, VIDEO_MIN_WIDTH);
	h = MAX(h, VIDEO_MIN_HEIGHT);
	vstate.output_w = w;
	vstate.output_h = h;
}


void kt_VideoOutputGet(u32 *w, u32 *h)
{
	*w = vstate.output_w;
	*h = vstate.output_h;
}


void kt_VideoFillModeSet(u32 fill_mode)
{
	vstate.fill_mode = fill_mode;
}

void kt_VideoFilterSet(u32 filter)
{
	vstate.filter_mode = filter;
}


void kt_VideoFrameSet(u32 frame)
{
	__kt_VideoAttrSet(VIDEO_ATTR_FRAME, &frame);
}


void kt_VideoTitleSet(const char* title)
{
	__kt_VideoAttrSet(VIDEO_ATTR_TITLE, (void*) title);
}
