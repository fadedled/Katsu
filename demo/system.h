/*
 * system.h
 */


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <katsu/kt.h>

void system_Init(u32 tmap);
void system_Clear();

void system_GetSystemChrOffs(u32 *chr_ofs, u32 *pal_ofs);

u32 system_WindowBegin(u32 x, u32 y, u32 width);
u32 system_WindowLabel(const char *str);
u32 system_WindowEnd();

void system_WindowMenuInit(const char *title, u32 x, u32 y, u32 width, const char **elems, u32 num_elems, KTSpr *cursor);
u32 system_WindowMenuUpdate(u32 was_selected, KTSpr *cursor);


#endif /*__SYSTEM_H__*/
