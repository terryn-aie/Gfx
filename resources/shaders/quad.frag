#version 450


in vec2 vUV;

layout(location = 0) uniform sampler2D map1;
layout(location = 1) uniform sampler2D map2;
layout(location = 2) uniform sampler2D map3;
layout(location = 3) uniform float time;

out vec4 outColor;


vec4 crossblur(in sampler2D map, in vec2 uv, int it);
vec4 sobel(in sampler2D map, in vec2 UV);

void main()
{

	//outColor = 4*sobel(map1, vUV) + 8*crossblur(map2, vUV, 64);

	//outColor = texture(map1, vUV) + sobel(map2,vUV);

	vec4 shimmer = length(sobel(map3, vUV)) * crossblur(map2, vUV,64);

	outColor = 2*cos(time/10+vUV.y) * shimmer + texture(map1, vUV);
}

vec4 crossblur(in sampler2D map, in vec2 uv, int it)
{
	vec2 sDim = textureSize(map,0).xy;
	vec4 retval = vec4(0,0,0,0);
	
	for(int i = -it; i <= it; ++i)
	{
		retval += texture(map, uv + vec2(i,0)/sDim);
		retval += texture(map, uv + vec2(0,i)/sDim);
	}

	return retval / (it*4);
}

vec4 sobel(in sampler2D map, in vec2 UV)
{
	vec2 sDim = textureSize(map,0).xy;
	vec4 yColor = 
	texture(map,UV + vec2( 1, -1)/sDim)   +
	texture(map,UV + vec2( 0, -1)/sDim)*2 +
	texture(map,UV + vec2(-1, -1)/sDim)   -
	texture(map,UV + vec2( 1,  1)/sDim)    -
	texture(map,UV + vec2( 0,  1)/sDim)*2  -
	texture(map,UV + vec2(-1,  1)/sDim);

	vec4 xColor = 
	texture(map,UV + vec2(-1, -1)/sDim)   +
	texture(map,UV + vec2(-1,  0)/sDim)*2 +
	texture(map,UV + vec2(-1,  1)/sDim)   -
	texture(map,UV + vec2( 1, -1)/sDim)   -
	texture(map,UV + vec2( 1,  0)/sDim)*2 -
	texture(map,UV + vec2( 1,  1)/sDim);

	return sqrt(yColor * yColor + xColor * xColor);
}