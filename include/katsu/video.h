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


#define KT_VIDEO_MIN_WIDTH     32						/*!< Minimum width of output. */
#define KT_VIDEO_MIN_HEIGHT    KT_VIDEO_MIN_WIDTH		/*!< Minimum height of output. */

#define KT_VIDEO_STD_WIDTH     424							/*!< Width of Standard definition. */
#define KT_VIDEO_STD_HEIGHT    256							/*!< Height of Standard definition. */
#define KT_VIDEO_HI_WIDTH      (KT_VIDEO_STD_WIDTH << 1)	/*!< Width of High definition. */
#define KT_VIDEO_HI_HEIGHT     (KT_VIDEO_STD_HEIGHT << 1)	/*!< Height of High definition. */



/*! \addtogroup fill_mode Fill modes
 * @{
 */
enum ViFillModes {
/*Fills with all the internal video (only output pixels are shown)*/
	KT_VIDEO_FILL_SCALE,		/*!< Preserves aspect ratio of KFB. */
	KT_VIDEO_FILL_STRECH,		/*!< Ignores aspect ratio of KFB. */
	KT_VIDEO_FILL_INTSCALE,		/*!< Uses integer scaling of KFB. */
/*Fills with only the output pixels*/
	KT_VIDEO_FILL_OUT_SCALE,	/*!< Preserves aspect ratio of output. */
	KT_VIDEO_FILL_OUT_STRECH,	/*!< Ignores aspect ratio of output. */
	KT_VIDEO_FILL_OUT_INTSCALE,	/*!< Uses integer scaling of output. */
	KT_VIDEO_MAX_FILL_MODES
};
/*! @} */

/*! \addtogroup frame_size Frame size
 * @{
 */
enum ViFrames {
	KT_VIDEO_FRAME_1X,
	KT_VIDEO_FRAME_2X,
	KT_VIDEO_FRAME_3X,
	KT_VIDEO_FRAME_4X,
	KT_VIDEO_FRAME_FULLSCREEN,
	KT_VIDEO_MAX_FRAME
};
/*! @} */


/*! \addtogroup res_mode Resolution mode
 * @{
 */
enum ViResolution {
	KT_VIDEO_RES_STD,	/*!< Sets standard definition. */
	KT_VIDEO_RES_HI,	/*!< Sets high definition. */
	KT_VIDEO_MAX_RES
};
/*! @} */

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
 * \brief Sets the size of the video output.
 * \details Both dimensions must be equal or greater than 32.
 * \ref kt_VideoSetResolution modifies the video output so this
 * function should be called afterwards.
 *
 * \param[in] w Width of the output.
 * \param[in] h Height of the output.
 */
void kt_VideoOutputSet(u32 w, u32 h);

/*!
 * \fn void kt_VideoOutputGet(u32 *w, u32 *h)
 * \brief Gets the size of the video output.
 * \details This function should only be used when a normal resolution is set
 *
 * \param[out] w Pointer where the width of the output will be stored.
 * \param[out] h Pointer where the height of the output will be stored.
 */
void kt_VideoOutputGet(u32 *w, u32 *h);

/*!
 * \fn void kt_VideoSetResolution(u32 res)
 * \brief Sets the resolution of the KFB.
 * \details There are two resolutions: Standard (424x256 pixels) and High (848x512 pixels). This function
 * should be used before \ref kt_VideoFrameSet and \ref kt_VideoOutputSet.
 * \warning Both resolutions have different rendering capabilities: Standard resolution
 * allows for 16 layers with 4 of them being rotation layer maps. High resolution only allows 8 layers and
 * 1 rotation layer map. It is best to clear all layers with \ref kt_LayerClearAll after using this function.
 *
 * \param[in] res \ref res_mode.
 */
void kt_VideoSetResolution(u32 res);

/*!
 * \fn void kt_VideoFillModeSet(u32 fill_mode)
 * \brief Used to specify how the video output will fill the frame.
 * \param[in] fill_mode \ref fill_mode.
 */
void kt_VideoFillModeSet(u32 fill_mode);

void kt_VideoFilterSet(u32 filter);

/*!
 * \fn void kt_VideoFrameSet(u32 frame)
 * \brief Sets the dimensions of the frame to the output values.
 * \details The frame is resized to these new dimensions, the current
 * output dimensions is used for calculating the new size so this function should
 * be called after using \ref kt_VideoSetResolution and \ref kt_VideoOutputSet.
 * \param[in] frame \ref frame_size.
 */
void kt_VideoFrameSet(u32 frame);

/*!
 * \fn void kt_VideoTitleSet(const char* title)
 * \brief Sets the title of the frame.
 *
 * \param[in] title String of the title.
 */
void kt_VideoTitleSet(const char* title);

/*!
 * \fn void kt_VideoHintFPS(u32 fps)
 * \brief Tries to set the FPS to a static framerate, note that
 * the systems refresh rate is a cap on the possible ranges.
 *
 * \param[in] fps The new number of frames per second.
 */
void kt_VideoHintFPS(u32 fps);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_VIDEO_H__*/
