
#version 450


layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform sampler2D shadowmap;

in vec3 vNormal;
in vec2 vUV;
in vec4 vPos;

uniform float shadowBias = 0.01;

// Simple matrix that converts from clip space (-1,1) to UV space (0,1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

out vec4 outColor;

float pcf(in sampler2D shadowMap, in vec4 shadowPosition, int iterations);

void main()
{
	// -1 to 1 range is in clip space, but the shadow buffer is in 0-1 UV space
	// [-1, 1] * .5 = [-.5,.5] + .5 = [0,1]
	vec4 sUV = clipToUV * lproj * lview * vPos;	

	float visibility = 1;
	if(texture(shadowmap,sUV.xy).r  < sUV.z - shadowBias)
		visibility = 0;

	float lamb = dot(-lview[2].xyz, vNormal);


	outColor = vec4(1,1,0,1) * visibility * lamb; 
}
