#version 450

layout(location = 0)in vec3 position;
layout(location = 2)in float lifetime;

out vec3 vPosition;
out float vLifetime;

void main()
{
    vPosition = position;
    vLifetime = lifetime;
}