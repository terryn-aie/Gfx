
// Graphics\resources\shaders\test.vert
#version 450

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;

layout(location = 1) uniform int idx = 0;
layout(location = 2) uniform int rows = 4;
layout(location = 3) uniform int cols = 4;

layout(location = 4) uniform float px;
layout(location = 5) uniform float py;

out vec2 vUV;

void main ()
{
	int c = idx % cols; // these aren't right!
	int r = idx / rows; // these aren't right!


	gl_Position = position / vec4(10,10,1,1) + vec4(px,py,0,0);


	vUV = (vec2(c, r) + texCoord) / vec2(rows, cols);
}

// 	setUniforms(sq, loc, tslot, tex, (int)(time*3) % 4, 4, 4);

// float time = context.getTime();

// (int)(time*3) % 4