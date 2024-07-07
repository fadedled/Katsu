/*
 * video_common.h
 */


#ifndef __KT_VIDEO_COMMON_H__
#define __KT_VIDEO_COMMON_H__


#define KT_VIDEO_ATTR_TITLE		0
#define KT_VIDEO_ATTR_FRAME		1

typedef struct VideoState_t {
	u32 output_w;
	u32 output_h;
	u32 frame_w;
	u32 frame_h;
	u32 fill_mode;
	u32 filter_mode;

	u32 frame_output_x;
	u32 frame_output_y;
	u32 frame_output_w;
	u32 frame_output_h;
} VideoState;

extern VideoState vstate;

u32  __kt_VideoInit(void);
void __kt_VideoExit(void);
void __kt_VideoAttrSet(u32 attr, void *val);
void __kt_VideoPoll(void);
void __kt_VideoSwapBuffers(void);


void __kt_UpdateOutputDims(void);

#endif /*__KT_VIDEO_COMMON_H__*/
