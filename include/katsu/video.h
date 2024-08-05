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


#define KT_VIDEO_MIN_WIDTH     32					/*!< Minimum width of output. */
#define KT_VIDEO_MIN_HEIGHT    KT_VIDEO_MIN_WIDTH	/*!< Minimum height of output. */

#define KT_VIDEO_STD_WIDTH     424					/*!< Width of Standard definition. */
#define KT_VIDEO_STD_HEIGHT    240					/*!< Height of Standard definition. */
#define KT_VIDEO_MDN_WIDTH     636					/*!< Width of Modern definition. */
#define KT_VIDEO_MDN_HEIGHT    360					/*!< Height of Modern definition. */
#define KT_VIDEO_HI_WIDTH      848					/*!< Width of High definition. */
#define KT_VIDEO_HI_HEIGHT     480					/*!< Height of High definition. */



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
	KT_VIDEO_RES_MDN,	/*!< Sets modern definition. */
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
 * \details There are three resolutions: Standard (240p), Modern (mostly
 * for applications that want more detailed images), High (480p). This function
 * should be used before \ref kt_VideoFrameSet and \ref kt_VideoOutputSet.
 * \warning Every resolution has it's own rendering capabilities: Standard definition
 * allows 16 layers and 4 rotation layer maps. Modern definition only allows 8 layers and
 * 2 rotation layer maps. High definition only allows 4 layers and no rotation layer maps.
 * It is best to clear all layers with \ref kt_LayerClearAll after using this function.
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
 * \brief Sets the dimensions of the frame to predefined values.
 * \details The frame is resized to these new dimensions, the current
 * resolution is used for calculating the new size so this function should
 * be called after using \ref kt_VideoSetResolution.
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

//XXX: High definition mode

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_VIDEO_H__*/
