

#include <katsu/kt.h>
#include <stdio.h>
#include <math.h>
#include "system.h"
#include "sequencer.h"
#include "samples/keyboard_mouse.h"



/*Demo Palette*/
extern const u8 norm_demo_4bpp_pal[];
extern const u8 affine_0_demo_4bpp_pal[];
extern const u8 affine_1_demo_4bpp_pal[];


/*Demo Tiles*/
extern const u8 norm_demo_4bpp_data[];
extern const u8 affine_0_demo_4bpp_data[];
extern const u8 affine_1_demo_4bpp_data[];

extern const u32 norm_demo_4bpp_tilenum;
extern const u32 affine_0_demo_4bpp_tilenum;
extern const u32 affine_1_demo_4bpp_tilenum;


extern u8 norm_tm_0_data[];
extern u8 norm_tm_1_data[];
extern u8 affine_tm_0_data[];
extern u8 affine_tm_1_data[];

extern u8 affine_bg_0_data[];
extern u32 affine_bg_w;
extern u32 affine_bg_h;


int main() {
	if (kt_Init()) {
		return 0;
	}
	kt_VideoTitleSet("Katsu Demo");
	kt_VideoFillModeSet(KT_VIDEO_FILL_SCALE);
	kt_VideoFrameSet(KT_VIDEO_FRAME_2X);
	system_Init(KT_TMAP15);

	samp_keymouse_init();
	while (1) {
		kt_Poll();
#if 0
		while (kt_KeyboardGetEvent(&kev)) {
			if (kev.type == KEYBOARD_EVTYPE_PRESSED) {
				//Print the UTF
				if(kev.sym > 0x7FF) {
					printf("%c%c%c", ((kev.sym >> 12) & 0xF) | 0xE0, (kev.sym >> 6 & 0x3F) | 0x80, (kev.sym & 0x3F) | 0x80);
				} else if(kev.sym > 0x7F) {
					printf("%c%c", ((kev.sym >> 6) & 0x1F) | 0xC0, (kev.sym & 0x3F) | 0x80);
				} else {
					printf("%c", kev.sym);
				}
				fflush(stdout);
			}
		}
#endif

		samp_keymouse_update();
		kt_Draw();
	}

	return 0;
}
