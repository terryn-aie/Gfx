
#version 450

out vec4 outColor;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;

layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 4) uniform sampler2D normalMap;
layout(location = 5) uniform sampler2D specularMap;

layout(location = 6) uniform vec3 lightDirection;
layout(location = 7) uniform vec4 lightColor;

void main()
{
	//vec3 L = normalize(vec3(1,-1,-1));
	vec3 N = vNormal.xyz;
	
	// diffuse lighting
	float lamb = dot(N, -L);

	outColor = lamb*texture(map,vUV) * col;
}