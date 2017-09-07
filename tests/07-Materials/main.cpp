#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\GameObjects.h"
#include "glm\ext.hpp"

int main()
{
	Context context;
	context.init(1280,720);

	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ {  1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ {  1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Shader standard = loadShader("../../resources/shaders/standard.vert",
								 "../../resources/shaders/standard.frag");

	Framebuffer screen = { 0, 1280, 720 };


	////////////////////////
	/// Model Data
	// first soulspear
	SpecGloss sceneObjects[2];
	sceneObjects[0].geo = loadGeometry("../../resources/models/soulspear.obj");

	sceneObjects[0].normal   = loadTexture("../../resources/textures/soulspear_normal.tga");
	sceneObjects[0].diffuse  = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	sceneObjects[0].specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	sceneObjects[0].gloss    = 4.0f;

	// second soulspear
	sceneObjects[1] = sceneObjects[0];
	sceneObjects[1].model = glm::rotate(90.f, glm::vec3(0, 1, 0));

	//////////////////////////
	// Camera Data
	Camera cam;
	cam.view = glm::lookAt(glm::vec3(0, 2,  3),
									 glm::vec3(0, 2,  0),
									 glm::vec3(0, 1,  0));
	cam.proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 5.f);	
	
	//////////////////////////
	// Light
	StandardLight l;
	l.dir       = glm::normalize(glm::vec3(.2, -1, -1));
	l.color     = glm::vec4(1.0, .5, 1.0, 1);
	l.intensity = 4.0;
	l.ambient   = glm::vec4(.2, .5, .1, 1);
	l.type      = 0;


	Framebuffer fBuffer = makeFramebuffer(1280, 720, 4, true, 3, 1);

	Shader fsq_shader = loadShader("../../resources/shaders/quad.vert",
								   "../../resources/shaders/quad.frag");
	while (context.step())
	{
		float time = context.getTime();
		sceneObjects[0].model = glm::rotate(time, glm::vec3(0, 1, 0));

		//////////////////////////////////////////////
		//////// Frame Buffer Pass
		clearFramebuffer(fBuffer);
		setFlags(RenderFlag::DEPTH);

		for (int i = 0; i < 2; ++i)
		{
			int loc = 0, slot = 0;
			setUniforms(standard, loc, slot,
				cam.proj, cam.view,	// Camera data!
				sceneObjects[i].model, sceneObjects[i].diffuse, sceneObjects[i].specular, sceneObjects[i].normal, sceneObjects[i].gloss, // model data!
				l.dir, l.color, l.intensity, l.ambient, l.type		  // light data!
			);

			s0_draw(fBuffer, standard, sceneObjects[i].geo);
		}

		//////////////////////////////////////////////
		//////// Screen Pass
		clearFramebuffer(screen);
		int loc = 0, slot = 0;
		setUniforms(fsq_shader, loc, slot, fBuffer.targets[1],
										   fBuffer.targets[2],
										   fBuffer.targets[3],
										   time);
		s0_draw(screen, fsq_shader, quad);
	}
	context.term();
	return 0;
}