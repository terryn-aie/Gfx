#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 vPosition[];
in float vLifetime[];

out vec4 gColor;

layout(location = 0)uniform mat4 projection;
layout(location = 1)uniform mat4 view;

layout(location = 2)uniform mat4 cameraTransform;

void main()
{
    gColor = vec4(1,1,1,1);

    float halfSize = 10 / 0.5f;

    vec3 corners[4];
    corners[0] = vec3(halfSize, -halfSize, 0);
    corners[1] = vec3(halfSize, halfSize, 0);
    corners[2] = vec3(-halfSize, -halfSize, 0);
    corners[3] = vec3(-halfSize, halfSize, 0);

    vec3 zAxis = normalize(cameraTransform[3].xyz - vPosition[0]);
    vec3 xAxis = cross(cameraTransform[1].xyz, zAxis);
    vec3 yAxis = cross(zAxis, xAxis);
    mat3 billboard = mat3(xAxis, yAxis, zAxis);

    for(int i = 0; i < 4; ++i)
    {
        gl_Position = projection * view * vec4(billboard * corners[i] + vPosition[0], 1);
        EmitVertex();
    }
}