#include "glinc.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\GameObjects.h"
#include <iostream>

ParticleSystem::ParticleSystem(unsigned int capacity, float lifetime, float interval)
{
	particleBuffer = new Particle[particleCapacity = capacity];

	defaultLifetime = lifetime;
	defaultColor = glm::vec4(1, 1, 1, 1);

	activeBufferIdx = 0;
	isInitialized = false;

	initialize();
}

ParticleSystem::~ParticleSystem()
{
	freeShader({ drawShader });
	freeShader({ updateShader });

	delete[] particleBuffer;
}

void ParticleSystem::initialize()
{
	// create shaders
	// - draw shader
	drawShader = loadShader("../../resources/shaders/particle.vert",
		"../../resources/shaders/particle.frag",
		"../../resources/shaders/particle.geom");

	// - update shader
	// TODO: this process is unusual for our pipeline
	//       so we'll do it here instead... :(
	//       ... need to refactor shader build pipeline

	// source
	std::string upd_src = fileToString("../../resources/shaders/particleUpdate.vert");

	const char * src = upd_src.c_str();

	// compile
	unsigned int shaderHandle = glCreateProgram();


	unsigned us = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(us, 1, &src, 0);

	glCompileShader(us);

#ifdef _DEBUG
	GLint success = GL_FALSE;

	glGetShaderiv(us, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(us, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(us, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif

	// link
	glAttachShader(shaderHandle, us);

	// transformFeedback
	const char * varyings[] = { "oPosition", "oVelocity", "oLifetime" };
	glTransformFeedbackVaryings(shaderHandle, 3, varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(shaderHandle);

#ifdef _DEBUG
	success = GL_FALSE;
	glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(shaderHandle, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif _DEBUG

	updateShader = { shaderHandle };

	// create buffers
	glGenVertexArrays(2, bufferVAO);
	glGenBuffers(2, bufferVBO);

	for (int i = 0; i < 2; ++i)
	{
		glBindVertexArray(bufferVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferVBO[i]);

		glBufferData(GL_ARRAY_BUFFER, particleCapacity * sizeof(Particle), particleBuffer, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)12);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)24);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	isInitialized = true;
}

void ParticleSystem::update(float deltaTime)
{
	assert(isInitialized && "Particle system was not initialized before updating.");

	runtime += deltaTime;

	int loc = 0, tex = 0;
	setUniforms(updateShader, loc, tex, runtime, deltaTime);

	unsigned int otherBuffer = (activeBufferIdx + 1) % 2;

	glEnable(GL_RASTERIZER_DISCARD);
	glBindVertexArray(bufferVAO[otherBuffer]);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferVBO[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, particleCapacity);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	activeBufferIdx = otherBuffer;
}

void ParticleSystem::draw(Camera& cam)
{
	assert(isInitialized && "Particle system was not initialized before updating.");

	int loc = 0, tex = 0;
	glBindVertexArray(bufferVAO[(activeBufferIdx + 1) % 2]);
	setUniforms(drawShader, loc, tex, cam, glm::inverse(cam.view));
	glDrawArrays(GL_POINTS, 0, particleCapacity);
}

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const Camera & val)
{
	t_setUniform(s, loc_io, tex_io, val.proj); // 0
	t_setUniform(s, loc_io, tex_io, val.view); // 1
}

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const SpecGloss & val)
{
	t_setUniform(s, loc_io, tex_io, val.model);	   // 0 matrix

	t_setUniform(s, loc_io, tex_io, val.diffuse);  // 1 texture
	t_setUniform(s, loc_io, tex_io, val.specular); // 2 texture
	t_setUniform(s, loc_io, tex_io, val.normal);   // 3 texture
	t_setUniform(s, loc_io, tex_io, val.gloss);    // 4 float
}

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const DirectionalLight & val)
{
	t_setUniform(s, loc_io, tex_io, val.getProj()); // 0
	t_setUniform(s, loc_io, tex_io, val.getView()); // 1
	t_setUniform(s, loc_io, tex_io, val.color);		// 2
	t_setUniform(s, loc_io, tex_io, val.intensity); // 3
}
