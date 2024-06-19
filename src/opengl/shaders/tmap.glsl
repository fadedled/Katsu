/*
 * bg.glsl - GLSL Shader for background drawing
 * ======================================
 * Uses glsl version 430
 */

layout(std140, binding = 0) uniform video_data
{
	vec2 frame;
	vec2 outdims;
	vec4 color_offset;
	vec4 mtx_mem[256];
};

#ifdef VERTEX_SHADER
//=====================================
// Vertex Shader
//=====================================
layout(location = 0) uniform uvec2 lay_rect;
layout(location = 1) uniform uvec4 map;


layout(location = 0) out vec2 uv;
layout(location = 1) out flat float blend;
layout(location = 2) out flat uvec2 map_size;
layout(location = 3) out flat uint tmap_num;
layout(location = 4) out flat uint tile_ofs;
layout(location = 5) out flat uint pal_ofs;

void main()
{
	const vec2 cl = vec2(1.0, 0.0);
	vec2 tmap_pos = vec2(lay_rect.x  & 0xFFFFu, lay_rect.x >> 16);
	vec2 tmap_size = vec2(lay_rect.y & 0xFFFFu, lay_rect.y >> 16);
	blend = float(map.x & 0xFF) / 255.0;
	uv = vec2(gl_VertexID & 1, gl_VertexID >> 1) * tmap_size;
	vec2 pos = ((uv + tmap_pos) / (vec2(424.0, -240.0) / 2.0)) + vec2(-1.0, 1.0);
	uv += vec2(uvec2(map.y, map.y >> 16) & 0xFFFFu);
	map_size = (map.xx >> uvec2(11, 12)) & 0x200u;
	tmap_num = (map.x >> 4) & 0xF000u;
	pal_ofs = (map.z >> 16) & 0xFFFFu;
	tile_ofs = map.z & 0xFFFFu;
	gl_Position = vec4(pos, 0.0, 1.0);
}

#endif
#ifdef FRAGMENT_SHADER
//=====================================
// Fragment Shader
//=====================================
layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) in vec2 uv;
layout(location = 1) in flat float blend;
layout(location = 2) in flat uvec2 map_size;
layout(location = 3) in flat uint tmap_num;
layout(location = 4) in flat uint tile_ofs;
layout(location = 5) in flat uint pal_ofs;


layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform usampler2D tile_mem;
layout(binding = 1) uniform usampler2D tmap_mem;
layout(binding = 2) uniform sampler2D pal_mem;



void main()
{
	//uvec4 bg = vc[layer + vc_ofs];
	//uvec2 mos = ((vc[0x4u + vc_ofs].rr >> uvec2(0u, 4u)) & 0xfu) + 1u;
	//uint norep = (-(bg.y & 0x200u)) << 1u;
	//uint tpage = bg.y & 0x400u;
	//blend = float(bg.y & 0xffu) / 255.0;
	//if (((bg.y >> 8u) & 1u) != has_blend) {
	//	discard;
	//}
	//ivec2 iuv = uv;
	//uvec4 m = u16tou32((bg.zzww >> uvec4(16u, 0u, 16u, 0u)) & 0xffffu);
	//uvec2 mpix = pix - (pix % mos);
	//uvec2 c = ((bg.yy >> uvec2(12u, 22u)) & 0x3ffu);
	//vec2 scale_p = uv * (1.0/ (pix_h + 1.0));
	//vec2 dp = (tmap_mat[layer_indx].xy * scale_p.xx) + (tmap_mat[layer_indx].zw * scale_p.yy);
	//ivec2 ofs = ivec2(floor(tmap_offset[layer_indx] + dp)); //+ c;

	//get tile
	uvec2 pix = uvec2(uv);//(((m.xz * ofs.xx) + (m.yw * ofs.yy)) >> 10u) + c;
	//if (bool((p.x | p.y) & 0x400u)) {
	//	discard;
	//}
	uint map_width = map_size.x >> 9;
	uint tmap_ofs = (((pix.x & map_size.x) + ((pix.y & map_size.y) << map_width)) << 3) + tmap_num;
	uint tl = ((pix.y & 0x1F8u) << 3u) + ((pix.x & 0x1F8u) >> 3u) + (tmap_ofs & 0xF000u);
	uvec4 tile = texelFetch(tmap_mem, ivec2(tl & 0xffu, tl >> 8u), 0);

	//get palette index
	uint pal = (tile.x + pal_ofs) & 0xFFu;
	uint flip_x = -((tile.z >> 6) & 1u);
	uint flip_y = -((tile.z >> 7) & 1u);
	uint chr = ((flip_y ^ pix.y) & 7u) + (((((tile.z << 8) | tile.w) + tile_ofs) & 0x3FFFu) << 3u);
	uint byte = ((flip_x ^ pix.x) >> 1) & 0x3;
	uint shft = ((flip_x ^ pix.x) & 1u) << 2u;
	uint idx = (texelFetch(tile_mem, ivec2(chr & 0xffu, chr >> 8u), 0)[byte] >> shft) & 0xfu;

	//Color 0 is transparent
	if (idx == 0u) {
		discard;
	}
	//get final color
	frag_color = vec4(texelFetch(pal_mem, ivec2(idx, pal), 0).rgb, blend);
}



#endif
