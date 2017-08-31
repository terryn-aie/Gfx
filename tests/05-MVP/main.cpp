

#include "graphics\Context.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\draw.h"
#include "glm\ext.hpp"
#include "graphics\Vertex.h"

void main()
{
	Context context;
	context.init();


	Geometry ss_geo = loadGeometry("../../resources/models/soulspear.obj");
	Shader mvplite = loadShader("../../resources/shaders/mvplite.vert",
								"../../resources/shaders/mvplite.frag");

	Texture ss_diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");

	Framebuffer screen = { 0, 800, 600 };
	glm::mat4 proj = glm::perspective(45.f, 800.f / 600.f, 0.01f, 10.f);
	glm::mat4 view = glm::lookAt(glm::vec3(0,2,5), glm::vec3(0,1,0), glm::vec3(0,1,0));
	

	while (context.step())
	{
		float time = context.getTime();
		glm::mat4 model = glm::rotate(time, glm::vec3(0, 1, 0));
		
		
		setFlags(RenderFlag::DEPTH);
		clearFramebuffer(screen);

		int loc = 0, slot = 0;
		setUniforms(mvplite, loc, slot, proj, view, model, ss_diffuse);
		s0_draw(screen, mvplite, ss_geo);
	}

	context.term();
}