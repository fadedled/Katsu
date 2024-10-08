/*
 * kt.h
 */

#ifndef __KT_H__
#define __KT_H__

/*!
 * \file kt.h
 * \brief Core Katsu library
 *
 */

#include <katsu/types.h>
#include <katsu/audio.h>
#include <katsu/video.h>
#include <katsu/gfx.h>
#include <katsu/input.h>
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

/*!
 * \mainpage
 * Welcome to the Katsu reference documentation.
 * The following list has detailed API descriptions of every subsystem in Katsu.
 *
 * - \ref kt.h "Core Katsu library"
 * - \ref types.h "Built-in type definitions"
 * - \ref video.h "VIDEO subsystem"
 * - \ref joypad.h "JOYPAD subsystem"
 * - \ref gfx.h "GFX subsystem"
 * - \ref audio.h "AUDIO subsystem"
 */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_H__*/
