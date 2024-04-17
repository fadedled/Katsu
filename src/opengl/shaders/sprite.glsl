/*
 * main.glsl - Main Display Vertex Shader
 * ======================================
 * Uses glsl version 430
 */

layout(std140, binding = 0) uniform video_data
{
	vec2 viewport;
	vec2 outsize;
	vec4 color_offset;
};

#ifdef VERTEX_SHADER
//=====================================
// Vertex Shader
//=====================================
layout(location = 0) in uvec4 sprite;

layout(location = 0) flat out uint tile;
layout(location = 1) flat out uint pal;
layout(location = 2) out vec2 uv;
layout(location = 3) flat out uint width;
layout(location = 4) flat out float blend;
layout(location = 5) flat out vec4 hue;

vec4 u16toRGBA(uint color, uint alpha) {
	//XXX: Should add the lower bits to rest of the byte
	uvec3 rgb = uvec3(color << 3, color >> 2, color >> 7) & 0xF8;
	return vec4(rgb, alpha) / 255.0;
}

/* SPRITE STRUCTURE
 * x  = [pos_y : 16][pos_x : 16]
 * y  = [pal : 8][vsize : 4][hsize : 4][vf : 1][hf : 1][tile_num : 14]
 * z  = [blend : 1][hue : 15][hue_alpha : 8][alpha : 8]
 * w  = [none : 24][mat_num : 8]
 */
void main()
{
	/*Get vertex position*/
	//XXX: add matrix transformation
	ivec2 vert = ivec2(gl_VertexID, gl_VertexID >> 1) & 0x1;
	ivec2 hsize = (((ivec2(sprite.y >> 16u, sprite.y >> 20u) & 0xf) << 2) + 4);
	uv = vec2((hsize * vert) << 1);
	uvec2 upos = uvec2(sprite.x & 0xFFFFu, sprite.x >> 16u);
	ivec2 ipos = ivec2((-(upos & 0x8000u)) | upos) + ivec2(uv);
	//XXX: use screen values from the uniform
	vec2 pos = (vec2(ipos) / (vec2(424.0, -240.0))) + vec2(-1.0, 1.0);
	gl_Position = vec4(pos, 0.0, 1.0);

	tile = (sprite.y & 0x3FFFu) << 3;
	pal = (sprite.y >> 24) & 0xFFu;
	width = uint(hsize.x >> 2);
	hue = u16toRGBA((sprite.z >> 16u) & 0x7FFFu, (sprite.z >> 8u) & 0xFFu);
	if ((sprite.z >> 31) == 1) {
		blend = float(sprite.z & 0xffu) / 255.0;
	} else {
		blend = 1.0;
	}
}

#endif
#ifdef FRAGMENT_SHADER
//=====================================
// Fragment Shader
//=====================================
layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) flat in uint tile;
layout(location = 1) flat in uint pal;
layout(location = 2) in vec2 uv;
layout(location = 3) flat in uint width;
layout(location = 4) flat in float blend;
layout(location = 5) flat in vec4 hue;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform usampler2D tile_mem;
layout(binding = 2) uniform sampler2D pal_mem;

void main()
{
	//XXX: implement depending on 4bpp or 8bpp
	uvec2 iuv = uvec2(uv);
	uint chy = (tile + (iuv.x & 0x78u) + ((iuv.y & 0x78u) * width)) + (iuv.y & 7u) & 0x1fffu;
	uint chx = ((iuv.x & 7u) << 2u);
	uint index = (texelFetch(tile_mem, ivec2(chy & 0xffu, chy >> 8u), 0).r >> chx) & 0xfu;
	//XXX: this must be done
	//if (index == 0u) {
	//	discard;
	//}
	vec4 spr_color = texelFetch(pal_mem, ivec2(pal, index), 0);
	//get final color
	frag_color = vec4(mix(spr_color, hue, hue.a).rgb, blend);
}

#endif
