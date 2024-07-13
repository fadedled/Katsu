/*
 * joypad.h
 */

/*!
 * \file joypad.h
 * \brief Katsu joypad subsystem
 *
 */


#ifndef __KT_JOYPAD_H__
#define __KT_JOYPAD_H__

#include <katsu/types.h>


/*! \addtogroup joypad_btn Joypad Buttons
 * @{
 */
#define JOY_LEFT       	0x0001u
#define JOY_RIGHT       0x0002u
#define JOY_UP          0x0004u
#define JOY_DOWN       	0x0008u
#define JOY_A       	0x0010u
#define JOY_B           0x0020u
#define JOY_X           0x0040u
#define JOY_Y       	0x0080u
#define JOY_L       	0x0100u
#define JOY_R           0x0200u
#define JOY_TL          0x0400u
#define JOY_TR       	0x0800u
#define JOY_STR       	0x1000u
#define JOY_SEL         0x2000u
/*! @} */

#define JOY_BIT_LEFT      0x0u
#define JOY_BIT_RIGHT     0x1u
#define JOY_BIT_UP        0x2u
#define JOY_BIT_DOWN      0x3u
#define JOY_BIT_A         0x4u
#define JOY_BIT_B         0x5u
#define JOY_BIT_X         0x6u
#define JOY_BIT_Y         0x7u
#define JOY_BIT_L         0x8u
#define JOY_BIT_R         0x9u
#define JOY_BIT_TL        0xAu
#define JOY_BIT_TR        0xBu
#define JOY_BIT_STR       0xCu
#define JOY_BIT_SEL       0xDu


/*! \addtogroup joypad_stick Joypad stick axis
 * @{
 */
#define JOY_STICK_LX    0x00u
#define JOY_STICK_LY    0x01u
#define JOY_STICK_RX    0x02u
#define JOY_STICK_RY    0x03u
/*! @} */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*! \addtogroup joypad_id Joypad ID
 * @{
 */
enum WinId {
	JOY0,
	JOY1,
	JOY2,
	JOY3,
	MAX_JOYPADS					/*!< Max number of joypads. */
};
/*! @} */



/*Joypad Functions*/
/*!
 * \fn u32  kt_JoyIsActive(u32 joy_id)
 * \brief Checks if specified joypad is connected.
 *
 * \param[in] joy_id \ref joypad_id.
 *
 * \return Returns non negative if joypad is connected, else it returns 0.
 */
u32  kt_JoyIsActive(u32 joy_id);

/*!
 * \fn u32  kt_JoyButtonHeld(u32 joy_id)
 * \brief Returns currently held buttons of specified joypad.
 *
 * \param[in] joy_id \ref joypad_id.
 *
 * \return On bits are buttons that are held.
 */
u32  kt_JoyButtonHeld(u32 joy_id);

/*!
 * \fn u32  kt_JoyButtonUp(u32 joy_id)
 * \brief Returns buttons of specified joypad that have just been released.
 *
 * \param[in] joy_id \ref joypad_id.
 *
 * \return On bits are buttons that have just been released.
 */
u32  kt_JoyButtonUp(u32 joy_id);

/*!
 * \fn u32  kt_JoyButtonDown(u32 joy_id)
 * \brief Returns buttons of specified joypad that have just been pushed.
 *
 * \param[in] joy_id \ref joypad_id.
 *
 * \return On bits are buttons that have just been pushed.
 */
u32  kt_JoyButtonDown(u32 joy_id);

/*!
 * \fn s8   kt_JoyStick(u32 joy_id, u32 stick_id)
 * \brief Returns value of the stick of the specified joypad.
 *
 * \param[in] joy_id \ref joypad_id.
 * \param[in] stick_id \ref joypad_stick.
 *
 * \return Returns the stick's axis value.
 */
s8   kt_JoyStick(u32 joy_id, u32 stick_id);

//XXX: Configure button mappings?
//XXX: Join stick and dpad?

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_OYPAD_H__*/
