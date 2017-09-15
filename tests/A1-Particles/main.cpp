#include "graphics\GameObjects.h"
#include "graphics\Vertex.h"
#include "graphics\Load.h"
#include "glm\ext.hpp"
#include "graphics\Context.h"
#include "graphics\draw.h"

int main()
{
	Context context;
	context.init(1280, 720);

	Framebuffer screen = {0,1280,720};

	ParticleSystem ps(1000, 3.0f, 0.5f);

	float prevTime = context.getTime();

	Camera cam;
	cam.view = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	cam.proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);

	while (context.step())
	{
		float deltaTime = context.getTime() - prevTime;
		prevTime = context.getTime();

		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		ps.update(deltaTime);
		ps.draw(cam);
	}
	context.term();

	return 0;
}