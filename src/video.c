
#include <katsu/kt.h>
#include "video_common.h"

VideoState vstate;


void kt_VideoOutputSet(u32 w, u32 h)
{
	w = MIN(w, KT_VIDEO_MAX_WIDTH);
	h = MIN(h, KT_VIDEO_MAX_HEIGHT);
	w = MAX(w, KT_VIDEO_MIN_WIDTH);
	h = MAX(h, KT_VIDEO_MIN_HEIGHT);
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
	if (fill_mode < KT_VIDEO_MAX_FILL_MODES) {
		vstate.fill_mode = fill_mode;
	}
}

void kt_VideoFilterSet(u32 filter)
{
	if (filter < KT_VIDEO_MAX_FILTER) {
		vstate.filter_mode = filter;
	}
}


void kt_VideoFrameSet(u32 frame)
{
	__kt_VideoAttrSet(KT_VIDEO_ATTR_FRAME, &frame);
}


void kt_VideoTitleSet(const char* title)
{
	__kt_VideoAttrSet(KT_VIDEO_ATTR_TITLE, (void*) title);
}
