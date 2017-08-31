
#include "graphics\Context.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"

#include "glm\ext.hpp"


int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[4] =
	{ { { -.75f, -.75f, .5, 1 },{ 1,1,0,1 },{ 0,0 } },
	{ { .75f, -.75f, .5, 1 },{ 0,1,1,1 },{ 1,0 } },
	{ { .75f,  .75f, .5, 1 },{ 1,0,1,1 },{ 1,1 } },
	{ { -.75f,  .75f, .5, 1 },{ 0,1,0,1 },{ 0,1 } } };

	unsigned idxs[3 * 2] = { 0,1,2, 0,2,3 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);

	unsigned char pixels[] = { 0,255,128,255,
		255,128,255,0,
		0,255,128,255,
		255,128,255,0 }; // 6 bytes instead of 4

	Texture t_mask = makeTexture(4, 4, 1, pixels);
	Texture t_magyel = makeTexture(4, 1, 3, pixels);

	Framebuffer f = { 0, 800, 800 };

	const char* vsource = // once per vertex
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 6) uniform mat4 model;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = model * position;\n"
		"}\n";

	const char* fsource = // once per pixel
		"#version 450\n"
		"out vec4 outColor;\n"

		"layout(location = 0) uniform int k0;\n"
		"layout(location = 1) uniform int k1;\n"
		"layout(location = 2) uniform int k2;\n"

		"layout(location = 3) uniform vec4 c0 = vec4(1,0,0,1);\n"
		"layout(location = 4) uniform vec4 c1 = vec4(0,1,0,1);\n"
		"layout(location = 5) uniform vec4 c2 = vec4(0,0,1,1);\n" // default values!


		"void main ()\n"
		"{\n"

		"outColor = c0*(1-k0) + c1*(1-k1) + c2*(1-k2); //vec4(k0, k1, k2, 1);\n"
		
		"}\n";

	Shader s = makeShader(vsource, fsource);

	while (context.step())
	{
		clearFramebuffer(f);
		glm::mat4 mod1 = glm::translate(glm::vec3(4.0, 0, 0)) *
						 glm::rotate((float)context.getTime(), glm::vec3(0.f, 0.f, 1.f)) *
						 glm::scale(glm::vec3(.6,.6,1));

		glm::mat4 mod2 = glm::rotate((float)context.getTime(), glm::vec3(0.f, 0.f, 1.f)) *
						 glm::scale(glm::vec3(.2, .2, 1));

		int loc = 0, tex = 0;
		setUniforms(s, loc, tex, (int)context.getKey('A'), (int)context.getKey('S'), (int)context.getKey('D'),
								 glm::vec4(1,1,1,1), glm::vec4(1,0,1,1), glm::vec4(.5,.5,.5,1),
								 mod2*mod1);
		s0_draw(f, s, g);

		loc = 0, tex = 0;
		setUniforms(s, loc, tex, (int)context.getKey('A'), (int)context.getKey('S'), (int)context.getKey('D'),
			glm::vec4(1, 1, 1, 1), glm::vec4(1, 0, 1, 1), glm::vec4(.5, .5, .5, 1),
			mod2);
		s0_draw(f, s, g);
	}


	freeTexture(t_magyel);
	freeTexture(t_mask);
	freeGeometry(g);
	freeShader(s);
	context.term();
}