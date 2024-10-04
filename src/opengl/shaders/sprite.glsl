/*
 * sprite.glsl - GLSL Shader for sprite drawing
 * ======================================
 * Uses glsl version 430
 */

layout(std140, binding = 0) uniform video_data
{
	vec2 maxdims;
	vec2 frame;
	vec2 outdims;
	vec2 _padding;
	vec4 color_offset;
	vec4 mtx_mem[256];
	vec3 linemap_data[1024*3];
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
	ivec2 vert = ivec2(gl_VertexID, gl_VertexID >> 1) & 0x1;
	ivec2 hsize = ((ivec2(sprite.yy >> uvec2(16u, 20u)) & 0xf) + 1) << 2;
	uv = vec2(((hsize << 1) * vert));
	/*Matrix transformation*/
	vec4 flip = 1.0 - vec4((sprite.y >> 13) & 2u, 0, 0, (sprite.y >> 14) & 2u);
	vec4 mtx = mtx_mem[sprite.w & 0xFFu] * flip;
	vec2 center_uv = uv - vec2(hsize);
	center_uv = vec2(dot(center_uv, mtx.xy), dot(center_uv, mtx.zw));
	uvec2 uofs = uvec2(sprite.x, sprite.x >> 16u) & 0xFFFFu;
	ivec2 iofs = ivec2((-(uofs & 0x8000u)) | uofs) + ivec2(center_uv) + hsize;
	//XXX: use screen values from the uniform
	vec2 pos = (vec2(iofs) / maxdims) + vec2(-1.0, 1.0);
	gl_Position = vec4(pos, 0.0, 1.0);

	tile = (sprite.y & 0x3FFFu) << 3;
	pal = (sprite.y >> 24) & 0x7Fu;
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
	uvec2 chr_hi = (uvec2(uv) & 0x78u) * uvec2(1, width);
	uvec2 chr_lo = uvec2(uv) & 0x7u;
	uint byte = chr_lo.x >> 1;
	uint shft = ((~chr_lo.x) & 0x1) << 2;
	uvec2 chr = uvec2(tile + chr_hi.x + chr_hi.y + chr_lo.y) & uvec2(0xFF, 0x3FFF);
	uint idx = (texelFetch(tile_mem, ivec2(chr.x, chr.y >> 8), 0)[byte] >> shft) & 0xfu;
	//Color 0 is transparent
	if (idx == 0u) {
		discard;
	}
	//get final color
	vec4 spr_color = texelFetch(pal_mem, ivec2(idx, pal), 0);
	frag_color = vec4(mix(spr_color, hue, hue.a).rgb, blend);
}

#endif
