

#include <katsu/kt.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl.h"
#include "../gfx_common.h"
#include "../video_common.h"


#include "shaders/main.inc"
#include "shaders/sprite.inc"
#include "shaders/tmap.inc"


struct VideoData_t {
	f32 maxdims_w;
	f32 maxdims_h;
	f32 frame_w;
	f32 frame_h;
	f32 outdims_w;
	f32 outdims_h;
	f32 _padding0;
	f32 _padding1;
	f32 color_offset_r;
	f32 color_offset_g;
	f32 color_offset_b;
	f32 color_offset_a;
} video_data;

u32 prog_bg;
u32 prog_spr;
u32 prog_win;
u32 prog_final;

u32 main_vao;
u32 main_vbo;
u32 video_data_ubo;

u32 tex_tiles;
u32 tex_bg;
u32 tex_pal;
u32 tex_win;
u32 tex_mfb;

u32 fb_win;
u32 fb_main;
u32 rb_depth;


KTSpr sprite_verts[KT_MAX_SPRITES * 4];
u32 vert_start[KT_MAX_SPRITES];
u32 vert_count[KT_MAX_SPRITES];
KTLineMapEntry linemap_data[KT_MAX_LINEMAP_LINES];

//Repeats all Sprite data (the same sprite data per vertex)
static void __kt_BuildVertexData(void)
{
	u32 count = 0;
	u32 linedata_ofs = sizeof(video_data);
	Layer *lr = layer_mem;
	for (u32 i = 0; i < (KT_MAX_LAYERS >> vstate.res_mode); ++i) {
		if (lr->type == KT_LAYER_SPRITE && lr->data_ptr) {
			KTSpr *spr = (KTSpr *) lr->data_ptr;
			for (u32 i = 0; i < lr->data_count && count < (KT_MAX_SPRITES * 4); ++i) {
				sprite_verts[count] = *spr;
				sprite_verts[count+1] = *spr;
				sprite_verts[count+2] = *spr;
				sprite_verts[count+3] = *spr;
				count += 4;
				++spr;
			}
		} else if (lr->type == KT_LAYER_MAP_NORMAL && lr->data_ptr) {
			//TODO: fill line data
			glBufferSubData(GL_UNIFORM_BUFFER, linedata_ofs, sizeof(KTLineMapEntry) * lr->data_count, lr->data_ptr);
			linedata_ofs += sizeof(KTLineMapEntry) * lr->data_count;
		}
		++lr;
	}
}


static u32 __kt_CreateSimpleGLProgram(const char *sh_src)
{

#ifdef KT_DEBUG
	s32 status;
	GLsizei log_length = 0;
	GLchar message[1024];
#endif //KT_DEBUG

	// Compile Vertex Shader
	u32 vsh_obj = glCreateShader(GL_VERTEX_SHADER);
	const char *vsh_source[2] = {"#version 430 core\n#define VERTEX_SHADER\n", sh_src};
	glShaderSource(vsh_obj, 2, vsh_source, NULL);
	glCompileShader(vsh_obj);

#ifdef KT_DEBUG
	glGetShaderiv(vsh_obj, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(vsh_obj, 1024, &log_length, message);
		printf("SHADER ERROR in Vertex Shader:\n%s\n", message);
		exit(0);
	}
#endif //KT_DEBUG

	// Compile Fragment Shader
	u32 fsh_obj = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fsh_source[2] = {"#version 430 core\n#define FRAGMENT_SHADER\n", sh_src};
	glShaderSource(fsh_obj, 2, fsh_source, NULL);
	glCompileShader(fsh_obj);

#ifdef KT_DEBUG
	glGetShaderiv(fsh_obj, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fsh_obj, 1024, &log_length, message);
		printf("SHADER ERROR in Fragment Shader:\n%s\n", message);
		exit(0);
	}
#endif //KT_DEBUG

	// Link program
	u32 program = glCreateProgram();
	glAttachShader(program, vsh_obj);
	glAttachShader(program, fsh_obj);
	glLinkProgram(program);

#ifdef KT_DEBUG
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(program, 1024, &log_length, message);
		printf("%s\n", message);
		exit(0);
	}
#endif //KT_DEBUG

	/* Delete shaders */
	glDetachShader(program, vsh_obj);
	glDetachShader(program, fsh_obj);
	glDeleteShader(vsh_obj);
	glDeleteShader(fsh_obj);

	return program;
}


static void __kt_BlendModeSet(u32 src, u32 dest, u32 func)
{
	const u32 func_arr[]= {GL_FUNC_ADD, GL_FUNC_SUBTRACT};
	const u32 factor_arr[]= {GL_ONE, GL_ZERO, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA};

	u32 gl_src = factor_arr[src];
	u32 gl_dest = factor_arr[dest];
	u32 gl_func = func_arr[func];

	glBlendEquation(gl_func);
	glBlendFunc(gl_src, gl_dest);
}


void ogl_Init(void)
{
	/*Set some OpenGL settings*/
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//XXX: Compile shaders

	/*Generate main VAO and VBO for sprites*/
	glGenVertexArrays(1, &main_vao);
	glBindVertexArray(main_vao);
	glGenBuffers(1, &main_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, main_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_verts), NULL, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &video_data_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, video_data_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(video_data) + (sizeof(KTLineMapEntry) * 1024), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 4, GL_UNSIGNED_INT, sizeof(KTSpr), (void*) 0);
	glEnable(GL_TEXTURE_2D);

	/*XXX: Uniform buffer object ???*/
#ifdef KT_DEBUG
	printf("%s GLSL: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Compiling Sprite Program\n");
#endif
	prog_spr = __kt_CreateSimpleGLProgram((const char*)sprite_glsl);
#ifdef KT_DEBUG
	printf("Compiling Tilemap Program\n");
#endif
	prog_bg = __kt_CreateSimpleGLProgram((const char*)tmap_glsl);
#ifdef KT_DEBUG
	printf("Compiling Main Program\n");
#endif
	prog_final = __kt_CreateSimpleGLProgram((const char*)main_glsl);


	/*Texture for tiles*/
	glGenTextures(1, &tex_tiles);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_tiles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8UI, 0x100, 0x200);
	/*Texture for backgrounds*/
	glGenTextures(1, &tex_bg);
	glBindTexture(GL_TEXTURE_2D, tex_bg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8UI, 0x100, 0x100);
	/*Texture for palettes*/
	glGenTextures(1, &tex_pal);
	glBindTexture(GL_TEXTURE_2D, tex_pal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, 0x10, 0x80);

	/*Texture for main framebuffer*/
	glGenTextures(1, &tex_win);
	glBindTexture(GL_TEXTURE_2D, tex_win);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, KT_VIDEO_HI_WIDTH, KT_VIDEO_HI_HEIGHT);

	glGenTextures(1, &tex_mfb);
	glBindTexture(GL_TEXTURE_2D, tex_mfb);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, KT_VIDEO_HI_WIDTH, KT_VIDEO_HI_HEIGHT);

	/*Gen a depth render buffer*/
	glGenRenderbuffers(1, &rb_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, KT_VIDEO_HI_WIDTH, KT_VIDEO_HI_HEIGHT);

	/*Gen a depth render buffer*/
	u32 drawbuffs[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
	glGenFramebuffers(1, &fb_win);
	glBindFramebuffer(GL_FRAMEBUFFER, fb_win);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_win, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
	glDrawBuffers(1, drawbuffs);
	//XXX: simple error check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Err: Window Framebuffer not completed\n");
		return;
	}

	glGenFramebuffers(1, &fb_main);
	glBindFramebuffer(GL_FRAMEBUFFER, fb_main);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_mfb, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
	glDrawBuffers(1, drawbuffs);
	//XXX: simple error check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Err: Main Framebuffer not completed\n");
		return;
	}

	glBindVertexArray(0);
	/*Init the arrays used fot glMultiDrawArrays*/
	for (u32 i = 0; i < KT_MAX_SPRITES; ++i) {
		vert_start[i] = i << 2;
		vert_count[i] = 4;
	}
}


void ogl_Draw(void)
{
	//XXX: do not do this here
	video_data.maxdims_w = (f32) (vstate.max_w >> 1);
	video_data.maxdims_h = -((f32) (vstate.max_h >> 1));
	video_data.frame_w = (f32) vstate.frame_w;
	video_data.frame_h = (f32) vstate.frame_h;
	video_data.outdims_w = (f32) vstate.output_w;
	video_data.outdims_h = (f32) vstate.output_h;
	video_data.color_offset_r = ((f32) S9TOS32(coloroffs) / 255.0);
	video_data.color_offset_g = ((f32) S9TOS32(coloroffs >> 9) / 255.0);
	video_data.color_offset_b = ((f32) S9TOS32(coloroffs >> 18) / 255.0);
	video_data.color_offset_a = 1.0;

	f32 backcol_r = (backcolor.r & 0xFF) / 255.0;
	f32 backcol_g = (backcolor.g & 0xFF) / 255.0;
	f32 backcol_b = (backcolor.b & 0xFF) / 255.0;

	//Update the buffers
	glBindVertexArray(main_vao);
	glBindBuffer(GL_UNIFORM_BUFFER, video_data_ubo);
	__kt_BuildVertexData();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_verts), sprite_verts);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(video_data), &video_data);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, video_data_ubo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_tiles);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0x100, 0x200, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, tile_mem);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, tex_bg);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0x100, 0x100, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, tmap_mem);
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, tex_pal);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0x10, 0x80, GL_RGBA, GL_UNSIGNED_BYTE, pal_mem);
	glActiveTexture(GL_TEXTURE0+3);
	glBindTexture(GL_TEXTURE_2D, tex_win);

	//Draw the windows

	//Draw all layers
	glBindFramebuffer(GL_FRAMEBUFFER, fb_main);
	glViewport(0, 0, vstate.max_w, vstate.max_h);
	glClearColor(backcol_r, backcol_g, backcol_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//XXX: Test
	u32 blnd_act = 0;
	glDisable(GL_BLEND);

	Layer *lr = layer_mem;
	u32 spr_indx = 0;
	u32 linemap_indx = 0;
	for (u32 i = 0; i < (KT_MAX_LAYERS >> vstate.res_mode); ++i) {
		//Set the blending mode for the layer
		u8 blnd = lr->blnd;
		__kt_BlendModeSet((blnd & 0x7), ((blnd >> 3) & 0x7), ((blnd >> 6) & 0x3));
		//Draw the layer
		switch (lr->type) {
		case KT_LAYER_MAP_NORMAL: {
			//Since blending is activated per spirte we configure on the fly
			glUseProgram(prog_bg);
			glUniform2ui(0, lr->rect_pos, lr->rect_size);
			glUniform2ui(1, lr->map_ofsx, lr->map_ofsy);
			glUniform4ui(2, lr->map_attr, 0, lr->chr_ofs, lr->map_scale);
			if (lr->data_ptr) {
				glUniform2ui(3, 256*linemap_indx, lr->data_count);
				linemap_indx++;
			} else {
				glUniform2ui(3, 0, 0);
			}
			if (blnd_act ^ lr->map_attr >> 31) {
				blnd_act = lr->map_attr >> 31;
				if (blnd_act) {
					glEnable(GL_BLEND);
				} else {
					glDisable(GL_BLEND);
				}
			}
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		} break;
		case KT_LAYER_SPRITE: {
			//Since blending is activated per spirte we configure on the fly
			glUseProgram(prog_spr);
			KTSpr *spr = (KTSpr *) lr->data_ptr;
			for (u32 j = 0; j < lr->data_count; ++j) {
				if (blnd_act ^ spr[j].sfx >> 31) {
					blnd_act = spr[j].sfx >> 31;
					if (blnd_act) {
						glEnable(GL_BLEND);
					} else {
						glDisable(GL_BLEND);
					}
				}
				glDrawArrays(GL_TRIANGLE_STRIP, spr_indx << 2, 4);
				++spr_indx;
			}
		} break;

		default: break;
		}
		++lr;
	}


	//Draw final image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(prog_final);
	glDisable(GL_BLEND);
	//XXX: Use the real viewport output
	glViewport(0, 0, vstate.frame_w, vstate.frame_h);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(vstate.frame_output_x, vstate.frame_output_y, vstate.frame_output_w, vstate.frame_output_h);
	//XXX: Upload final color/width/hegiht
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_mfb);

	u32 filter_mode = (vstate.frame_w > vstate.max_w * 2  || vstate.frame_h > vstate.max_h * 2 ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
