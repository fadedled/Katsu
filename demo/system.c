
#include <katsu/kt.h>

extern const u8 system_4bpp_pal[];
extern const u8 system_4bpp_data[];
extern const u32 system_4bpp_tilenum;

u32 sys_tmap;
u32 win_x;
u32 win_y;
u32 win_width;
u32 sys_pal_ofs;
u32 sys_chr_ofs;

void system_Init(u32 tmap)
{
	sys_chr_ofs = KT_MAX_TILES - system_4bpp_tilenum;
	sys_pal_ofs = 127;
	kt_TilesetLoad(sys_chr_ofs, system_4bpp_tilenum , system_4bpp_data);
	kt_PaletteLoad(sys_pal_ofs, KT_PAL_SIZE_16, system_4bpp_pal);
	sys_tmap = tmap;
}

void system_GetSystemChrOffs(u32 *chr_ofs, u32 *pal_ofs) {
	*chr_ofs = sys_chr_ofs;
	*pal_ofs = sys_pal_ofs;
}

void system_Clear()
{
	for (u32 y = 0; y < 30; y++) {
		for (u32 x = 0; x < 54; x++) {
			kt_TilemapSetChr(sys_tmap, x, y, 0, KT_FLIP_NONE, 0);
		}
	}

}

u32 system_WindowBegin(u32 x, u32 y, u32 width)
{
	win_x = x;
	win_y = y;
	win_width = width;
	if (!win_width)
		return 0;

	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, KT_FLIP_Y, 0);
	for (u32 i = 1; i < win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, KT_FLIP_Y, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, KT_FLIP_XY, 0);
	win_y++;
	return 1;
}



u32 system_WindowLabel(char *str)
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 5, KT_FLIP_NONE, 0);
	u32 str_end = 0;
	for (u32 i = 1; i < win_width; ++i) {
		if (!str_end) {
			if (*str == '\0') {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, ' ', KT_FLIP_NONE, 0);
				str_end = 1;
			} else {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, *str, KT_FLIP_NONE, 0);
			}
			str++;
		} else {
			kt_TilemapSetChr(sys_tmap, win_x + i, win_y, ' ', KT_FLIP_NONE, 0);
		}
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 5, KT_FLIP_X, 0);
	win_y++;
	return 1;
}

u32 system_WindowEnd()
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, KT_FLIP_NONE, 0);
	for (u32 i = 1; i< win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, KT_FLIP_NONE, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, KT_FLIP_X, 0);
	win_width = 0;
	return 1;
}
