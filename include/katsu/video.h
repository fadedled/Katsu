/*
 * video.h
 */

/*!
 * \file video.h
 * \brief Katsu VIDEO subsystem
 *
 */

#ifndef __KT_VIDEO_H__
#define __KT_VIDEO_H__

#include <katsu/types.h>


#define KT_VIDEO_STD_WIDTH     424
#define KT_VIDEO_STD_HEIGHT    240
#define KT_VIDEO_MIN_WIDTH     32
#define KT_VIDEO_MIN_HEIGHT    KT_VIDEO_MIN_WIDTH

#define KT_VIDEO_MDN_WIDTH     636
#define KT_VIDEO_MDN_HEIGHT    360

#define KT_VIDEO_HI_WIDTH      848
#define KT_VIDEO_HI_HEIGHT     480



/*Fill Modes*/
enum ViFillModes {
/*Fills with all the internal video (only output pixels are shown)*/
	KT_VIDEO_FILL_SCALE,		/*Preserves aspect ratio*/
	KT_VIDEO_FILL_STRECH,		/*Ignores aspect ratio*/
	KT_VIDEO_FILL_INTSCALE,		/*Uses integer scaling*/
/*Fills with only the output pixels*/
	KT_VIDEO_FILL_OUT_SCALE,
	KT_VIDEO_FILL_OUT_STRECH,
	KT_VIDEO_FILL_OUT_INTSCALE,
	KT_VIDEO_MAX_FILL_MODES
};


/*Frame Size*/
enum ViFrames {
	KT_VIDEO_FRAME_1X,
	KT_VIDEO_FRAME_2X,
	KT_VIDEO_FRAME_3X,
	KT_VIDEO_FRAME_4X,
	KT_VIDEO_FRAME_FULLSCREEN,
	KT_VIDEO_MAX_FRAME
};


/*Resolution mode*/
enum ViResolution {
	KT_VIDEO_RES_STD,
	KT_VIDEO_RES_MDN,
	KT_VIDEO_RES_HI,
	KT_VIDEO_MAX_RES
};


/*Filter type*/
enum ViFilters {
	KT_VIDEO_FILTER_SHARP,
	KT_VIDEO_FILTER_SOFT,
	KT_VIDEO_FILTER_CRT,
	KT_VIDEO_FILTER_CRT_SOFT,
	KT_VIDEO_FILTER_LCD,
	KT_VIDEO_FILTER_CUSTOM,
	KT_VIDEO_MAX_FILTER
};


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ============================================================================
 * Video Functions
 * ============================================================================
 */
/*!
 * \fn void kt_VideoOutputSet(u32 w, u32 h)
 * \brief Sets the size of the output.
 * \details Both dimensions must be equal or greater than 32.
 *
 * \param[in] w Width of the output.
 * \param[in] h Height of the output.
 */
void kt_VideoOutputSet(u32 w, u32 h);

/*!
 * \fn void kt_VideoOutputGet(u32 *w, u32 *h)
 * \brief Gets the size of the output.
 * \details This function should only be used when a normal resolution is set
 *
 * \param[out] w Pointer where the width of the output will be stored.
 * \param[out] h Pointer where the height of the output will be stored.
 */
void kt_VideoOutputGet(u32 *w, u32 *h);

void kt_VideoSetResolution(u32 res);

void kt_VideoFillModeSet(u32 fill_mode);
void kt_VideoFilterSet(u32 filter);
void kt_VideoFrameSet(u32 frame);
void kt_VideoTitleSet(const char* title);

//XXX: High definition mode

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_VIDEO_H__*/
