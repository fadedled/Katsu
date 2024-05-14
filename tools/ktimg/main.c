
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* Integers */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef int32_t  s32;


u8 pal[16*4] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x22, 0x22, 0x22, 0x00, 0x33, 0x33, 0x33,
	0x00, 0x44, 0x44, 0x44, 0x00, 0x55, 0x55, 0x55, 0x00, 0x66, 0x66, 0x66, 0x00, 0x77, 0x77, 0x77,
	0x00, 0x88, 0x88, 0x88, 0x00, 0x99, 0x99, 0x99, 0x00, 0xAA, 0xAA, 0xAA, 0x00, 0xBB, 0xBB, 0xBB,
	0x00, 0xCC, 0xCC, 0xCC, 0x00, 0xDD, 0xDD, 0xDD, 0x00, 0xEE, 0xEE, 0xEE, 0x00, 0xFF, 0xFF, 0xFF
};

//u32 *pal = bw_pal;

static u32 pal_MinDistanceColor(u32 r, u32 g, u32 b)
{
	u32 min_distance = ~0u;
	u32 idx = 0;
	//check all colors in palette
	for (u32 i = 0; i < 16; ++i) {
		u32 pal_r = pal[4*i + 3];
		u32 pal_g = pal[4*i + 2];
		u32 pal_b = pal[4*i + 1];
		if ((pal_r == r) & (pal_g == g) & (pal_b == b)) {
			return i;
		}
		//Eucledian distance
		s32 dr = (s32) ((pal_r) - (r));
		s32 dg = (s32) ((pal_g) - (g)) >> 8;
		s32 db = (s32) ((pal_b) - (b)) >> 16;
		u32 dist = (u32) sqrt((dr * dr) + (dg * dg) + (db * db));
		if (min_distance > dist) {
			min_distance = dist;
			idx = i;
		}
	}
	return idx;
}


void conv_4bpp(char* out_file, u32 w, u32 h, u8 *data)
{
	u32 size = w * h / 2;
	u8 *out_data = (u8 *) calloc(size, 1);
	for (u32 j = 0; j < h; j++) {
		for (u32 i = 0; i < w; i++) {
			u32 idx = pal_MinDistanceColor(data[0], data[1], data[2]) << ((i & 1) * 4);
			u32 pos = (i & 7) + (((j & 7) + ((j & 0xFFF8) * (w/8)) + (i & 0xFFF8)) << 3);
			pos >>= 1;
			out_data[pos] |= idx;
			data+= 4;
		}
	}
	// Write the image.
	FILE *fp = fopen(out_file, "wb");
	fwrite(out_data, 1, size, fp);
	fclose(fp);
	free(out_data);
}

void conv_8bpp(char* out_file, u32 w, u32 h, u8 *data)
{

}

void cstr_ChangeFilenameExt(char* in_fn, char* out_fn, char* file_ext)
{
	char *src_c = in_fn;
	char *dst_c = out_fn;
	char *dst_ext = NULL;
	while (*src_c) {
		if (*src_c == '.') {
			dst_ext = dst_c;
		}
		*dst_c = *src_c;
		src_c++;
		dst_c++;
	}

	if (!dst_ext) {
		dst_ext = dst_c;
	}

	*dst_ext = '.';
	dst_ext++;
	while (*file_ext) {
		*dst_ext = *file_ext;
		dst_ext++;
		file_ext++;
	}

	*dst_ext = '\0';
}


int main(int argc, char* argv[])
{
	char output_filename[256];
	char *filename;
	char *file_ext = "4bpp";
	int x, y, n;
	u8 *data;
	if (argc < 2) {
		printf("Katsu Image Converter:\n"
				"Usage: %s [options] <Input File>\n"
				"[options]:\n"
				//"\t-4bpp: Convert to 4bpp tiled image.\n"
				//"\t-8bpp: Convert to 8bpp tiled image.\n"
				"\t-pal <Input Palette>: Use palette for paletization.\n", argv[0]);
	}

	for (u32 i = 1; i < argc-1; i++) {
		if (!strcmp("-pal", argv[i])) {
			i++;
			// Obtain palette.
			FILE *fp = fopen(argv[i], "rb");
			if (!fp) {
				printf("%s: Palette file %s does not exist.", argv[0], argv[i]);
				return -1;
			}
			fread(pal, 1, 16*4, fp);
			fclose(fp);
		} else if (!strcmp("-8bpp", argv[i])) {
			file_ext = "8bpp";
		}
	}

	filename = argv[argc-1];
	cstr_ChangeFilenameExt(filename, output_filename, file_ext);
	data = stbi_load(filename, &x, &y, &n, 4);
	if (data) {
		conv_4bpp(output_filename, x, y, data);
	}
	stbi_image_free(data);

	return 0;
}
