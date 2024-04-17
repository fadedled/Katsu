/*
 * main display program shader
 */

#define TOSRC(x) #x

char glsl_main[2] = {TOSRC(
//=====================================================
//Vertex shader
//=====================================================
uniform vec2 viewport;
uniform vec2 outsize;

out vec2 uv;
out vec2 scale;
out vec2 range;

main()
{
	const vec2 cl = vec2(1.0, 0.0);
	vec2 p = vec2(uint(gl_VertexID) & 1u, uint(gl_VertexID) >> 1u);
	gl_Position = vec4(((p * 2.0) - 1.0), 0.0, 1.0);
	uv = p.xy * outsize;
	scale = max(floor(viewport / outsize), cl.xx);
	range = 0.5 - (0.5 / scale);
}


), TOSRC(
//=====================================================
//Fragment shader
//=====================================================
layout(origin_upper_left) in vec4 gl_FragCoord;

in vec2 uv;
in vec2 scale;
in vec2 range;

layout(location = 0) out vec4 frag_color;

uniform sampler2D tex_main;
uniform vec4 color_offset;
uniform vec2 outsize;

vec2 sharp_uv()
{
	vec2 center = fract(uv) - 0.5;
	vec2 f = ((center - clamp(center, -range, range)) * scale) + 0.5;
	return ((floor(uv) + f) / outsize);
}

main()
{
	frag_color = texture(tex_main, sharp_uv()) + color_offset;
}
)};
