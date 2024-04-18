/*
 * video.h
 */


#ifndef __KT_VIDEO_H__
#define __KT_VIDEO_H__

#include <katsu/types.h>


#define VIDEO_MAX_WIDTH     424
#define VIDEO_MAX_HEIGHT    240
#define VIDEO_MIN_WIDTH     32
#define VIDEO_MIN_HEIGHT    VIDEO_MIN_WIDTH


/*Fill Modes*/
enum ViFillModes {
/*Fills with all the internal video (only output pixels are shown)*/
	VIDEO_FILL_SCALE,		/*Preserves aspect ratio*/
	VIDEO_FILL_STRECH,		/*Ignores aspect ratio*/
	VIDEO_FILL_INTSCALE,	/*Uses integer scaling*/
/*Fills with only the output pixels*/
	VIDEO_FILL_OUT_SCALE,
	VIDEO_FILL_OUT_STRECH,
	VIDEO_FILL_OUT_INTSCALE,
	VIDEO_MAX_FILL_MODES
};


/*Frame Size*/
enum ViFrames {
	VIDEO_FRAME_1X,
	VIDEO_FRAME_2X,
	VIDEO_FRAME_3X,
	VIDEO_FRAME_4X,
	VIDEO_FRAME_FULLSCREEN,
	VIDEO_MAX_FRAME
};


/*Filter type*/
enum ViFilters {
	VIDEO_FILTER_SHARP,
	VIDEO_FILTER_SOFT,
	VIDEO_FILTER_CRT,
	VIDEO_FILTER_CRT_SOFT,
	VIDEO_FILTER_LCD,
	VIDEO_FILTER_CUSTOM,
	VIDEO_MAX_FILTER
};


/*Video Functions*/
void kt_VideoOutputSet(u32 w, u32 h);
void kt_VideoOutputGet(u32 *w, u32 *h);

void kt_VideoFillModeSet(u32 fill_mode);
void kt_VideoFilterSet(u32 filter);
void kt_VideoFrameSet(u32 frame);
void kt_VideoTitleSet(const char* title);

//XXX: High definition mode


#endif /*__KT_VIDEO_H__*/
