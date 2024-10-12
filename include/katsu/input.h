/*
 * input.h
 */

/*!
 * \file input.h
 * \brief Katsu INPUT subsystem (keyboard and mouse)
 *
 */


#ifndef __KT_INPUT_H__
#define __KT_INPUT_H__

#include <katsu/types.h>


/*! \addtogroup keyboard_evtype Keyboard Event Types
 * @{
 */
#define KEYBOARD_EVTYPE_PRESSED       0u
#define KEYBOARD_EVTYPE_RELEASED      1u
/*! @} */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct KTKeyEvent_t{
	u8  type;
	u8  keycode;
	u16 mod;	//Modifiers
	u16 sym;	//Leave?
} KTKeyEvent;


typedef struct KTMouse_t{
	s16 y;
	s16 x;
	s16 dy;
	s16 dx;
	u8 btn0;
	u8 btn1;
	s8 scroll;
} KTMouse;


/*Keyboard Functions*/
void kt_KeyboardEnablePolling(u32 enable);
u32 kt_KeyboardGetEvent(KTKeyEvent *kev);
u32 kt_KeyboardFlushEvents(void);

/*Mouse Functions*/
void kt_MouseEnablePolling(u32 enable);
u32 kt_MouseGetState(KTMouse *mouse);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_INPUT_H__*/
