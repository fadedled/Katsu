/*
 * main.glsl - GLSL Shader for main display drawing
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
	uvec4 linemap_data[512];
};

#ifdef VERTEX_SHADER
//=====================================
// Vertex Shader
//=====================================
layout(location = 0) out vec2 uv;
layout(location = 1) out vec2 scale;
layout(location = 2) out vec2 range;

void main()
{
	const vec2 cl = vec2(1.0, 0.0);
	vec2 p = vec2(uint(gl_VertexID) & 1u, uint(gl_VertexID) >> 1u);
	gl_Position = vec4(((p * 2.0) - 1.0), 0.0, 1.0);
	uv = p.xy * outdims;
	scale = max(floor(frame / vec2(848, 480)), cl.xx);
	range = 0.5 - (0.5 / scale);
}

#endif
#ifdef FRAGMENT_SHADER
//=====================================
// Fragment Shader
//=====================================
layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 scale;
layout(location = 2) in vec2 range;

layout(location = 0) out vec4 frag_color;
layout(binding = 0) uniform sampler2D tex_main;

vec2 sharp_uv()
{
	vec2 center = fract(uv) - 0.5;
	vec2 f = ((center - clamp(center, -range, range)) * scale) + 0.5;
	return ((floor(uv) + f) / vec2(848, 480));
}

void main()
{
	frag_color = texture(tex_main, sharp_uv()) + color_offset;
}

#endif
