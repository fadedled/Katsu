

#include <katsu/kt.h>
#include <GL/gl.h>

void ogl_Init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);

	//XXX: Compile shaders
}


f32 r;
f32 g;
f32 b;

void ogl_Draw(void)
{
	glViewport(0, 0, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT);
	r += 0.0078125;
	g += 0.00390625;
	b += 0.015625;
	r = r > 1.0 ? 0.0 : r;
	g = g > 1.0 ? 0.0 : g;
	b = b > 1.0 ? 0.0 : b;

	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
