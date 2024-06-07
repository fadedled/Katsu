/*
 * kt.h
 */

/*!
 * \file kt.h
 * \brief Katsu general system
 *
 */

#ifndef __KT_H__
#define __KT_H__


#include <katsu/types.h>
#include <katsu/audio.h>
#include <katsu/video.h>
#include <katsu/gfx.h>
#include <katsu/joypad.h>


/*! \addtogroup error_code Error codes
 * @{
 */
#define KT_OK					0x00000000u
#define KT_ERROR_NO_WINDOW		0x00000001u
#define KT_ERROR_NO_JOYPAD		0x00000002u
#define KT_ERROR_NO_AUDIO		0x00000003u
/*! @} */

#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define MIN(a, b)		((a) > (b) ? (b) : (a))

enum KtExitCode {
	KT_EXIT,
	KT_EXIT_CANCEL,
	KT_EXIT_MAX_CODES
};

typedef u32 (*KTExitFunc)(u32 status);

/*Main Katsun functions*/
u32 kt_Init(void);
void kt_ExitFuncSet(KTExitFunc func);
void kt_Exit(u32 status);
void kt_Poll(void);
void kt_Draw(void);




#endif /*__KT_H__*/
