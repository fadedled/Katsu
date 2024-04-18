

#include <katsu/kt.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl.h"
#include "../gfx_common.h"


#include "shaders/main.inc"
#include "shaders/sprite.inc"


struct VideoData_t {
	f32 viewport_w;
	f32 viewport_h;
	f32 outsize_w;
	f32 outsize_h;
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

/*
extern u32 tile_mem[0x20000];
extern u32 pal_mem[0x800];
extern u32 bg_mem[0x10000];
extern mat mat_mem[0x100];
*/

#define KT_DEBUG 1
Sprite sprite_verts[MAX_SPRITES * 4];
u32 vert_start[MAX_SPRITES];
u32 vert_count[MAX_SPRITES];

static void __kt_BuildVertexData(void)
{
	Sprite spr = {0};
	Sprite spr2 = {0};
	spr.pos = SPR_POS(0, 0);
	spr.tile = SPR_TILE(0, 0, 16, 24, 0);
	spr.sfx = SPR_HUE(0xFFFF, 0x00);// | SPR_BLEND(0x80);
	spr.mat = 0;

	spr2.pos = SPR_POS(60, 120);
	spr2.tile = SPR_TILE(0, 0, 32, 16, 0);
	spr2.sfx = SPR_HUE(0x00FF, 0x00);
	spr2.mat = 0;

	sprite_verts[0] = spr;
	sprite_verts[1] = spr;
	sprite_verts[2] = spr;
	sprite_verts[3] = spr;
	sprite_verts[4] = spr2;
	sprite_verts[5] = spr2;
	sprite_verts[6] = spr2;
	sprite_verts[7] = spr2;
#if 0
	u32 count = 0;
	Layer *lr = layer_mem;
	for (u32 i = 0; i < MAX_LAYERS; ++i) {
		if (lr->type == LAYER_TYPE_SPRITE) {
			Sprite *spr = lr->spr;
			for (u32 i = 0; i < lr->spr_count && count < (MAX_SPRITES * 4); ++i) {
				sprite_verts[count] = *spr;
				sprite_verts[count+1] = *spr;
				sprite_verts[count+2] = *spr;
				sprite_verts[count+3] = *spr;
				count += 4;
			}
		}
		++lr;
	}
#endif
}


static u32 __kt_CreateSimpleGLProgram(char *sh_src)
{

#if KT_DEBUG
	s32 status;
	GLsizei log_length = 0;
	GLchar message[1024];
#endif //KT_DEBUG

	// Compile Vertex Shader
	u32 vsh_obj = glCreateShader(GL_VERTEX_SHADER);
	const char *vsh_source[2] = {"#version 450 core\n#define VERTEX_SHADER\n", sh_src};
	glShaderSource(vsh_obj, 2, vsh_source, NULL);
	glCompileShader(vsh_obj);

#if KT_DEBUG
	glGetShaderiv(vsh_obj, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(vsh_obj, 1024, &log_length, message);
		printf("SHADER ERROR in Vertex Shader:\n%s\n", message);
		exit(0);
	}
#endif //KT_DEBUG

	// Compile Fragment Shader
	u32 fsh_obj = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fsh_source[2] = {"#version 450 core\n#define FRAGMENT_SHADER\n", sh_src};
	glShaderSource(fsh_obj, 2, fsh_source, NULL);
	glCompileShader(fsh_obj);

#if KT_DEBUG
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

	#if KT_DEBUG
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
	const u32 factor_arr[]= {GL_ZERO, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA};

	u32 gl_src = factor_arr[src];
	u32 gl_dest = factor_arr[src];
	u32 gl_func = func_arr[src];

	glBlendEquation(gl_func);
	glBlendFunc(gl_src, gl_dest);
}


void ogl_Init(void)
{
	/*Set some OpenGL settings*/
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
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
	glBufferData(GL_UNIFORM_BUFFER, sizeof(video_data), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 4, GL_UNSIGNED_INT, sizeof(Sprite), (void*) 0);
	glEnable(GL_TEXTURE_2D);

	/*XXX: Uniform buffer object ???*/
	#if KT_DEBUG
	printf("%s GLSL: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Compiling Sprite Program\n");
	#endif
	prog_spr = __kt_CreateSimpleGLProgram(sprite_glsl);
	#if KT_DEBUG
	printf("Compiling Main Program\n");
	#endif
	prog_final = __kt_CreateSimpleGLProgram(main_glsl);


	/*Texture for tiles*/
	glGenTextures(1, &tex_tiles);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_tiles);
	glTextureParameteri(tex_tiles, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(tex_tiles, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureStorage2D(tex_tiles, 1, GL_R32UI, 0x100, 0x200);
	/*Texture for backgrounds*/
	glGenTextures(1, &tex_bg);
	glBindTexture(GL_TEXTURE_2D, tex_bg);
	glTextureParameteri(tex_bg, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(tex_bg, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureStorage2D(tex_bg, 1, GL_R32UI, 0x100, 0x100);
	/*Texture for palettes*/
	glGenTextures(1, &tex_pal);
	glBindTexture(GL_TEXTURE_2D, tex_pal);
	glTextureParameteri(tex_pal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(tex_pal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureStorage2D(tex_pal, 1, GL_RGBA8, 0x10, 0x80);

	/*Texture for main framebuffer*/
	glGenTextures(1, &tex_win);
	glBindTexture(GL_TEXTURE_2D, tex_win);
	glTextureParameteri(tex_win, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(tex_win, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureStorage2D(tex_win, 1, GL_RGBA8, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);

	glGenTextures(1, &tex_mfb);
	glBindTexture(GL_TEXTURE_2D, tex_mfb);
	glTextureParameteri(tex_mfb, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(tex_mfb, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureStorage2D(tex_mfb, 1, GL_RGBA8, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);

	/*Gen a depth render buffer*/
	glGenRenderbuffers(1, &rb_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);

	/*Gen a depth render buffer*/
	u32 drawbuffs[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
	glGenFramebuffers(1, &fb_win);
	glBindFramebuffer(GL_FRAMEBUFFER, fb_win);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_win, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
	glDrawBuffers(1, drawbuffs);
	//XXX: simple error check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Err: Window Framebuffer not completed\n");
		return;
	}

	glGenFramebuffers(1, &fb_main);
	glBindFramebuffer(GL_FRAMEBUFFER, fb_main);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_mfb, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
	glDrawBuffers(1, drawbuffs);
	//XXX: simple error check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Err: Main Framebuffer not completed\n");
		return;
	}

	glBindVertexArray(0);
	/*Init the arrays used fot glMultiDrawArrays*/
	for (u32 i = 0; i < MAX_SPRITES; ++i) {
		vert_start[i] = i << 2;
		vert_count[i] = 4;
	}
}


void ogl_Draw(void)
{
	//XXX: do not do this here
	video_data.viewport_w = (f32) VIDEO_MAX_WIDTH;
	video_data.viewport_h = (f32) VIDEO_MAX_HEIGHT;
	video_data.outsize_w = (f32) VIDEO_MAX_WIDTH;
	video_data.outsize_h = (f32) VIDEO_MAX_HEIGHT;
	video_data.color_offset_r = (f32) (((coloroffs >> 0) & 0xFFu) / 255.0);
	video_data.color_offset_g = (f32) (((coloroffs >> 8) & 0xFFu) / 255.0);
	video_data.color_offset_b = (f32) (((coloroffs >> 16) & 0xFFu) / 255.0);
	video_data.color_offset_a = (f32) (((coloroffs >> 24) & 0xFFu) / 255.0);

	f32 backcol_r = (backcolor & 0xFF) / 255.0;
	f32 backcol_g = ((backcolor>>8) & 0xFF) / 255.0;
	f32 backcol_b = ((backcolor>>16) & 0xFF) / 255.0;

	//Update the buffers
	glBindVertexArray(main_vao);
	__kt_BuildVertexData();
	glBindBuffer(GL_UNIFORM_BUFFER, video_data_ubo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_verts), sprite_verts);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(video_data), &video_data);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, video_data_ubo);

	glTextureSubImage2D(tex_tiles, 0, 0, 0, 0x100, 0x200, GL_RED_INTEGER, GL_UNSIGNED_INT, tile_mem);
	glTextureSubImage2D(tex_bg, 0, 0, 0, 0x100, 0x100, GL_RED_INTEGER, GL_UNSIGNED_INT, bg_mem);
	glTextureSubImage2D(tex_pal, 0, 0, 0, 0x10, 0x80, GL_RGBA, GL_UNSIGNED_BYTE, pal_mem);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_tiles);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, tex_bg);
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, tex_pal);
	glActiveTexture(GL_TEXTURE0+3);
	glBindTexture(GL_TEXTURE_2D, tex_win);

	//Draw the windows
	__kt_BlendModeSet(KT_BLEND_ONE, KT_BLEND_ZERO, BLEND_FUNC_ADD);

	//Draw all layers
	glBindFramebuffer(GL_FRAMEBUFFER, fb_main);
	glViewport(0, 0, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);
	glClearColor(backcol_r, backcol_g, backcol_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_BLEND);
	//XXX: Test
	glUseProgram(prog_spr);
	glMultiDrawArrays(GL_TRIANGLE_STRIP, vert_start, vert_count, 2);

	Layer *lr = layer_mem;
	for (u32 i = 0; i < MAX_LAYERS; ++i) {
		//XXX: Add windows...

		//Draw the layer
		switch (lr->type) {
			case LAYER_TYPE_SPRITE: {


				//glMultiDrawArrays(GL_TRIANGLE_STRIP, 0, lr->count, 4);
			} break;
			default: break;
		}
		++lr;
	}
	glDisable(GL_BLEND);
	//Draw final image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(prog_final);

	//XXX: Use the real viewport output
	glViewport(0, 0, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);
	//XXX: Upload final color/width/hegiht
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_mfb);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
