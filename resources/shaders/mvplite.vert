
#version 450

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec4 normal;
layout(location = 4) in vec4 tangent;
layout(location = 5) in vec4 bitangent;

out vec2 vUV;
out vec4 vPos;
out vec4 vNormal;

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;


out mat4 TBN;

void main()
{
	gl_Position = proj * view * model * position;

	TBN = model * mat4(tangent,bitangent,normal,vec4(0,0,0,1));

	vPos = model * position;
	vNormal = model * normal;
	vUV = texcoord;
}