#pragma once

#include "RenderObjects.h"
#include "glm\ext.hpp"




// 0: model, 1: diffuse, 2: specular, 3: normal, 4: gloss
struct SpecGloss
{
	Geometry geo;

	// uniforms
	glm::mat4 model;		
	Texture diffuse;
	Texture specular;
	Texture normal;
	float gloss;
};

// 0: proj, 1: view
struct Camera
{
	glm::mat4 proj, view;
};

// 0: proj, 1: view, 2: color, 3: intensity
struct DirectionalLight
{
	glm::vec3 center; // location from which the shadow map is drawn.
	float size;		  // area that the shadow map covers.

	glm::vec3 direction;
	glm::vec4 color;
	float intensity;

	glm::mat4 getView() const { return glm::lookAt(direction + center, center, glm::vec3(0, 1, 0)); }
	glm::mat4 getProj() const { return glm::ortho<float>(-size, size, -size, size, -size, size); }
};


struct SimplePresetScene
{
	Camera cam;
	SpecGloss go[3];
	DirectionalLight dl[2];

	SimplePresetScene();
};



namespace __internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const DirectionalLight &val);
}