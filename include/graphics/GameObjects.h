#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "RenderObjects.h"

struct Camera
{
	glm::mat4 proj;
	glm::mat4 view;
};

struct SpecGloss
{
	Geometry geo;

	glm::mat4 model;
	Texture diffuse;
	Texture specular;
	Texture normal;
	float gloss;
};

struct StandardLight
{
	glm::vec3 dir;
	glm::vec4 color;
	float intensity;
	glm::vec4 ambient;
	int type;
};

struct DirectionalLight
{
	glm::vec3 target; // for shadow mapping
	float range;	  // for shadow mapping
	
	glm::vec3 direction;

	// 0
	glm::mat4 getProj() const
	{
		return glm::ortho<float>(-range, range, -range, range, -range, range);
	}

	// 1
	glm::mat4 getView() const
	{
		return glm::lookAt(-direction, glm::vec3(0,0,0), glm::vec3(0,1,0));
	} 

	glm::vec4 color; // 2
	float intensity; // 3
};

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
};

class ParticleSystem
{
	void initialize();
	bool isInitialized;

public:
	ParticleSystem(unsigned int capacity, float lifetime, float interval);
	~ParticleSystem();

	unsigned int particleCapacity;
	Particle * particleBuffer;

	float runtime;

	float spawnInterval;
	float spawnAccumulator;

	float defaultLifetime;
	glm::vec4 defaultColor;

	unsigned int activeBufferIdx;
	unsigned int bufferVAO[2];
	unsigned int bufferVBO[2];

	Shader drawShader;
	Shader updateShader;

	void update(float deltaTime);
	void draw(Camera& cam);
};

namespace __internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const DirectionalLight &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const ParticleSystem &val);
}