/*
 * types.h
 */


#ifndef __KT_TYPES_H__
#define __KT_TYPES_H__


#include <stdint.h>


/* Integers */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

/* Volatile integers */
typedef volatile u8    vu8;
typedef volatile u16   vu16;
typedef volatile u32   vu32;
typedef volatile u64   vu64;
typedef volatile s8    vs8;
typedef volatile s16   vs16;
typedef volatile s32   vs32;
typedef volatile s64   vs64;

/* Floating point */
typedef float          f32;
typedef double         f64;
typedef volatile f32   vf32;
typedef volatile f64   vf64;


#define KT_TRUE       1
#define KT_FALSE      0
#define KT_ENABLE     1
#define KT_DISABLE    0


//XXX: bool? guards?

#endif /*__KT_TYPES_H__*/
