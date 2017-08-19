
#include "graphics\Context.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"


int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[4] =
	{ { { -.75f, -.75f, .5, 1 },{ 1,1,0,1 },{0,0}},
	{ { .75f, -.75f, .5, 1 },{ 0,1,1,1 },{1,0} },
	{ { .75f,  .75f, .5, 1 },{ 1,0,1,1 },{1,1} },
	{ { -.75f,  .75f, .5, 1 },{ 0,1,0,1 },{0,1} } };

	unsigned idxs[3 * 2] = { 0,1,2, 0,2,3 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);

	const char* vsource = // once per vertex
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 2) in vec2 uv;\n"
		"layout(location = 0) uniform float time;\n"
		"layout(location = 2) uniform float xpos;\n"
		"layout(location = 3) uniform float ypos;\n"
		"out vec4 vColor;\n"
		"out vec2 vUV;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = position;\n"
		//"gl_Position.x += xpos;\n"
		//"gl_Position.y += ypos;\n"
		//"gl_Position.y += sin(time+position.x)/5.0;\n"
		"vColor = color;\n"
		"vUV = uv;\n"
		"}\n";

	const char* fsource = // once per pixel
		"#version 450\n"
		"out vec4 outColor;\n"
		"layout(location = 0) uniform float time;\n"
		//"layout(location = 1) uniform int tog;\n"
		"layout(location = 4) uniform sampler2D map;\n"
		"layout(location = 5) uniform sampler2D mask;\n"
		"in vec4 vColor;\n"
		"in vec2 vUV;\n"
		"void main ()\n"
		"{\n"
		"vec2 uv = vUV;\n"
		"uv.x += sin(time + uv.y);\n"
		"outColor = texture(map,uv) * texture(mask,uv + vec2(0,cos(time+uv.x))).r;\n"
		//"outColor = vec4(0,0,0,1);\n"
		//"if(tog == 1)\n"
		//"outColor = 1.0 - vColor;\n"
		//"else outColor = vColor;\n"
		//"outColor *= 1.0+cos(time*sin(time)+gl_FragCoord.y/10)/2.0+cos(time+gl_FragCoord.y/8)+sin(time*20+gl_FragCoord.x/5)/2.0;\n"
		//"vec2 det = vec2(20.0,20.0);\n"
		//"outColor *= 1.0-distance(mod(gl_FragCoord.xy,det),det/2.0)/15.0;"
		//"\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = { 0, 800, 800 };

	// RGB texture (3 channels), 2x2
	unsigned char pixels[] = {0,255,128,255,
							  255,128,255,0,
							  0,255,128,255,
							  255,128,255,0}; // 6 bytes instead of 4

	Texture t_mask = makeTexture(4, 4, 1, pixels);
	Texture t_magyel = makeTexture(4, 1, 3,pixels);

	glm::vec2 pos = { 0,0 };
	float prevTime = 0;
	float speed = 1.2f;
	while (context.step())
	{
		float ct = context.getTime();
		float dt = ct - prevTime;
		prevTime = ct;

		glm::vec2 vel = { 0,0 };
		vel.y += context.getKey('W');
		vel.x -= context.getKey('A');
		vel.y -= context.getKey('S');
		vel.x += context.getKey('D');

		if(glm::length(vel) > 0)
			pos += glm::normalize(vel) * dt * speed;

		clearFramebuffer(f);
		
		setUniform(s, 0, (float)context.getTime());
		setUniform(s, 1, context.getKey(' '));
		setUniform(s, 2, pos.x);
		setUniform(s, 3, pos.y);

		setUniform(s, 4, t_magyel, 0);
		setUniform(s, 5, t_mask,   1);

		s0_draw(f, s, g);
	}

	freeGeometry(g);
	freeShader(s);
	freeTexture(t_magyel);
	freeTexture(t_mask);
	context.term();
	return 0;
}