
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
	__kt_UpdateOutputDims();
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
	__kt_UpdateOutputDims();
}


void kt_VideoTitleSet(const char* title)
{
	__kt_VideoAttrSet(KT_VIDEO_ATTR_TITLE, (void*) title);
}


void __kt_UpdateOutputDims(void)
{
	u32 out_w = 0, out_h = 0;
	switch (vstate.fill_mode) {
		case KT_VIDEO_FILL_SCALE: {
			f32 aspect = (f32) KT_VIDEO_MAX_WIDTH / (f32) KT_VIDEO_MAX_HEIGHT;
			f32 frame_aspect = (f32) vstate.frame_w / (f32) vstate.frame_h;
			if (aspect <= frame_aspect) {
				out_h = vstate.frame_h;
				out_w = out_h * aspect;
			} else {
				out_w = vstate.frame_w;
				out_h = out_w / aspect;
			}
			out_w = (vstate.output_w * out_w) / KT_VIDEO_MAX_WIDTH;
			out_h = (vstate.output_h * out_h) / KT_VIDEO_MAX_HEIGHT;
		} break;
		case KT_VIDEO_FILL_STRECH: {
			out_w = vstate.frame_w;
			out_h = vstate.frame_h;
			out_w = (vstate.output_w * out_w) / KT_VIDEO_MAX_WIDTH;
			out_h = (vstate.output_h * out_h) / KT_VIDEO_MAX_HEIGHT;
		} break;
		case KT_VIDEO_FILL_INTSCALE: {
			out_w = KT_VIDEO_MAX_WIDTH * (vstate.frame_w / KT_VIDEO_MAX_WIDTH);
			out_h = KT_VIDEO_MAX_HEIGHT * (vstate.frame_h / KT_VIDEO_MAX_HEIGHT);
			out_w = (vstate.output_w * out_w) / KT_VIDEO_MAX_WIDTH;
			out_h = (vstate.output_h * out_h) / KT_VIDEO_MAX_HEIGHT;
		} break;
		case KT_VIDEO_FILL_OUT_SCALE: {
			f32 aspect = (f32) vstate.output_w / (f32) vstate.output_h;
			f32 frame_aspect = (f32) vstate.frame_w / (f32) vstate.frame_h;
			if (aspect <= frame_aspect) {
				out_h = vstate.frame_h;
				out_w = out_h * aspect;
			} else {
				out_w = vstate.frame_w;
				out_h = out_w / aspect;
			}
		} break;
		case KT_VIDEO_FILL_OUT_STRECH: {
			out_w = vstate.frame_w;
			out_h = vstate.frame_h;
		} break;
		case KT_VIDEO_FILL_OUT_INTSCALE: {
			out_w = vstate.output_w * (vstate.frame_w / vstate.output_w);
			out_h = vstate.output_h * (vstate.frame_h / vstate.output_h);
		} break;
	}

	vstate.frame_output_x = (vstate.frame_w - out_w) / 2;
	vstate.frame_output_y = (vstate.frame_h - out_h) / 2;
	vstate.frame_output_w = out_w;
	vstate.frame_output_h = out_h;
}
