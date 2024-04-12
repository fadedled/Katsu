/*
 * video_common.h
 */


#ifndef __KT_VIDEO_COMMON_H__
#define __KT_VIDEO_COMMON_H__


u32  __kt_VideoInit(void);
void __kt_VideoExit(void);
void __kt_VideoAttrSet(u32 attr, u32 val);
void __kt_VideoPoll(void);
void __kt_VideoSwapBuffers(void);

#endif /*__KT_VIDEO_COMMON_H__*/
