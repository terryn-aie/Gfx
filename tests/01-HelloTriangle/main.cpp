
#include "graphics\Context.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"

#include "genShape.h"

int main()
{
	Context context;
	context.init(800,800);

	// make all of our openGL stuff
	Vertex verts[4] = 
	{ { { -.5f, -.5f, .5, 1 }, {1,1,0,1} }, // bot-left    - 0
      { {  .5f, -.5f, .5, 1 }, {0,1,1,1} }, // top-left    - 1
	  { {  .5f,  .5f, .5, 1 }, {1,0,1,1} }, // top-right   - 2
	  { { -.5f,  .5f, .5, 1 }, {1,1,1,1} } }; // bot-right - 3

	//0, 1, 2
	unsigned idxs[3*2] = {0,1,2,
						  0,2,3};

	Geometry g = makeGeometry(verts, 4, idxs, 6);
	Geometry gt = makeNGon(5, .5f);

	const char* vsource = // once per vertex
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"out vec4 vColor;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = position;\n"
		"vColor = color;\n"
		"}\n";

	const char* fsource = // once per pixel
		"#version 450\n"
		"out vec4 outColor;\n"
		"in vec4 vColor;\n"
		"void main ()\n"
		"{\n"
		"outColor = vColor;\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = {0, 800, 800};

	while (context.step())
	{
		s0_draw(f, s, g);
		s0_draw(f, s, gt);
	}

	freeGeometry(g);
	freeShader(s);

	context.term();
	return 0;
}