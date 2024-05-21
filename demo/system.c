
#include <katsu/kt.h>


extern u8 _binary_res_tiles_system_tiles_4bpp_start[];
extern u8 _binary_res_tiles_system_tiles_4bpp_end[];
extern u8 _binary_res_tiles_system_spr_4bpp_start[];
extern u8 _binary_res_tiles_system_spr_4bpp_end[];

u32 sys_tmap;
u32 win_x;
u32 win_y;
u32 win_width;


void system_Init(u32 tmap)
{
	u32 sys_tile_size =  ((u64)(&_binary_res_tiles_system_tiles_4bpp_end) - (u64)(&_binary_res_tiles_system_tiles_4bpp_start)) / 32;
	kt_TilesetLoad(0, sys_tile_size, &_binary_res_tiles_system_tiles_4bpp_start);
	sys_tmap = tmap;
}

void system_Clear()
{
	for (u32 y = 0; y < 64; y++) {
		for (u32 x = 0; x < 64; x++) {
			kt_TilemapSetChr(sys_tmap, x, y, 0, FLIP_NONE, 0);
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

	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, FLIP_Y, 0);
	for (u32 i = 1; i < win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, FLIP_Y, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, FLIP_XY, 0);
	win_y++;
	return 1;
}

u32 system_WindowLabel(char *str)
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 5, FLIP_NONE, 0);
	u32 str_end = 0;
	for (u32 i = 1; i < win_width; ++i) {
		if (!str_end) {
			if (*str >= 'A' && *str <= 'Z') {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, *str - 'A' + 16, FLIP_NONE, 0);
			} else {
				kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 43, FLIP_NONE, 0);
			}
			if (*str == '\0') {
				str_end = 1;
			}
			str++;
		} else {
			kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 43, FLIP_NONE, 0);
		}
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 5, FLIP_X, 0);
	win_y++;
	return 1;
}

u32 system_WindowEnd()
{
	if (!win_width)
		return 0;
	kt_TilemapSetChr(sys_tmap, win_x, win_y, 3, FLIP_NONE, 0);
	for (u32 i = 1; i< win_width; ++i) {
		kt_TilemapSetChr(sys_tmap, win_x + i, win_y, 4, FLIP_NONE, 0);
	}
	kt_TilemapSetChr(sys_tmap, win_x + win_width, win_y, 3, FLIP_X, 0);
	win_width = 0;
	return 1;
}
