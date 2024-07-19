
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Integers */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef int32_t  s32;


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
		if (*src_c == '/') {
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


u32 file_GetSize(FILE *fp)
{
	u32 size = 0;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}
	return size;
}


int main(int argc, char* argv[])
{
	char output_filename[256];
	char file_base[256];
	char *filename;
	u32 skip_comments = 0;
	char *file_ext = "inc";
	if (argc < 2) {
		printf("Katsu Binary to inc file converter:\n"
				"Usage: %s [-nc] <Input File>\n"
				"\t-s : Skip C style one line comments."
				"\t-c : Set extension to .c.", argv[0]);
		return 0;
	}

	for (u32 i = 1; i < argc-1; i++) {
		if (strcmp(argv[1], "-s") == 0) {
			skip_comments = 1;
		} else if (strcmp(argv[1], "-c") == 0) {
			file_ext = "c";
		}
	}

	filename = argv[argc-1];
	cstr_ChangeFilenameExt(filename, output_filename, file_ext);
	cstr_GenStrVariableName(filename, file_base);

	// Read the file.
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		printf("%s Error: File %s not found!\n", argv[0], argv[1]);
		return -1;
	}
	u32 file_size = file_GetSize(fp);
	unsigned char *data = (unsigned char *) calloc(file_size, 1);
	fread(data, 1, file_size, fp);
	fclose(fp);

	unsigned char *src_data = data;
	unsigned char *dst_data = data;
	u32 will_copy = 1;
	u32 new_size = 0;
	for (u32 i = 0; i < file_size; i++) {
		if (skip_comments) {
			if ((*src_data == '/' && *(src_data+1) == '/')
				|| (*src_data == '\\' && *(src_data+1) == '\\')) {
				will_copy = 0;
			}
			if (!will_copy && *src_data == '\n') {
				will_copy = 1;
			}
		}
		if (will_copy) {
			*dst_data = *src_data;
			new_size++;
			dst_data++;
		}
		src_data++;
	}

	// Write the image.
	fp = fopen(output_filename, "w");
	u32 row = 0;
	fprintf(fp, "unsigned char %s[] = {\n\t", file_base);
	for (u32 i = 0; i < new_size; ++i) {
		fprintf(fp, "0x%02X, ", data[i]);
		row++;
		if (row == 16) {
			row = 0;
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "0x00\n};\n");
	fprintf(fp, "unsigned int %s_size = %d;\n", file_base, new_size+1);
	fclose(fp);
	free(data);

	return 0;
}
