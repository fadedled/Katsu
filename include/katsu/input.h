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

typedef struct KeyEvent_t{
	u8  type;
	u8  keycode;
	u16 mod;	//Modifiers
	u16 sym;	//Leave?
} KeyEvent;



/*Keyboard Functions*/
u32 kt_KeyboardGetEvent(KeyEvent *kev);
u32 kt_KeyboardFlushEvents(void);

/*Mouse Functions*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_INPUT_H__*/
