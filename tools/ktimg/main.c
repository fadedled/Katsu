
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

u32 c_file = 0;
u32 export_pal = 0;

//u32 *pal = bw_pal;


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

void cstr_GenStrVariableName(char* in_fn, char* out_fn)
{
	char *src_c = in_fn;
	char *fn_start = in_fn;
	while (*src_c) {
		if (*src_c == '/' || *src_c == '\\') {
			fn_start = src_c+1;
		}
		src_c++;
	}

	while (*fn_start) {
		if (*fn_start == '.') {
			*out_fn = '_';
		} else {
			*out_fn = *fn_start;
		}
		out_fn++;
		fn_start++;
	}

	*out_fn = '\0';
}


static void write_data(char* out_file,u8 *data, u32 size)
{
	// Write the image as a C source file.
	if (c_file) {
		char name[256] = {0};
		char cfilename[256] = {0};
		cstr_GenStrVariableName(out_file, name);
		cstr_ChangeFilenameExt(out_file, cfilename, "c");
		FILE *fp = fopen(cfilename, "w");
		//Generate symbol name from file


		fprintf(fp, "#include <katsu/types.h>\n\n");
		fprintf(fp, "const u8 %s_data[%d] = {\n", name, size);
		for (u32 i = 0; i < size; i += 8) {
			fprintf(fp, "\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n",
					data[i  ], data[i+1], data[i+2], data[i+3],
					data[i+4], data[i+5], data[i+6], data[i+7]);
		}

		fprintf(fp, "};\n\n");
		fprintf(fp, "const u32 %s_size = %d;\n", name, size);
		fprintf(fp, "const u32 %s_tilenum = %d;\n", name, size / 32);
		if (export_pal) {
			fprintf(fp, "const u8 %s_pal[16*4] = {\n", name);
			for (u32 i = 0; i < 16 * 4; i += 8) {
				fprintf(fp, "\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n",
						pal[i  ], pal[i+1], pal[i+2], pal[i+3],
						pal[i+4], pal[i+5], pal[i+6], pal[i+7]);
			}
			fprintf(fp, "};\n\n");
		}
		fclose(fp);
	} else {	// Write the image as binary file.
		FILE *fp = fopen(out_file, "wb");
		fwrite(data, 1, size, fp);
		fclose(fp);
	}
}


static u32 pal_MinDistanceColor(u32 r, u32 g, u32 b)
{
	u32 min_distance = ~0u;
	u32 idx = 0;
	//check all colors in palette
	for (u32 i = 0; i < 16; ++i) {
		u32 pal_r = pal[4*i    ];
		u32 pal_g = pal[4*i + 1];
		u32 pal_b = pal[4*i + 2];
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
	write_data(out_file, out_data, size);
	free(out_data);
}

void conv_8bpp(char* out_file, u32 w, u32 h, u8 *data)
{

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
				"\t-c: Output as C source file.\n"
				"\t-palexp: Export palette inside C file.\n"
				"\t-p <Palette image file>: Use palette for paletization.\n", argv[0]);
		return 0;
	}

	for (u32 i = 1; i < argc-1; i++) {
		if (!strcmp("-p", argv[i])) {
			int x, y, n;
			u8 *data;
			i++;
			// Obtain palette.
			data = stbi_load(argv[i], &x, &y, &n, 4);
			if (data) {
				memcpy(pal, data, 16*4);
			} else {
				printf("%s - Error: Palette file %s does not exist.\n", argv[0], argv[i]);
				return -1;
			}
			stbi_image_free(data);

		} else if (!strcmp("-8bpp", argv[i])) {
			file_ext = "8bpp";
		} else if (!strcmp("-c", argv[i])) {
			c_file = 1;
		} else if (!strcmp("-palexp", argv[i])) {
			export_pal = 1;
		}
	}

	if (!c_file) {
		export_pal = 0;
	}

	filename = argv[argc-1];
	cstr_ChangeFilenameExt(filename, output_filename, file_ext);
	data = stbi_load(filename, &x, &y, &n, 4);
	if ((x & 7) | (y & 7)) {
		printf("Error: The dimensions of image %s are not multiples of 8.\n", filename);
	} else {
		if (data) {
			conv_4bpp(output_filename, x, y, data);
		}
	}
	stbi_image_free(data);

	return 0;
}
