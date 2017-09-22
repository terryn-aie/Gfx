#version 450

layout(location = 0)in vec3 iPosition;
layout(location = 1)in vec3 iVelocity;
layout(location = 2)in float iLifetime;

out vec3 oPosition;
out vec3 oVelocity;
out float oLifetime;

layout(location = 0)uniform float time;
layout(location = 1)uniform float deltaTime;

layout(location = 2)uniform float defaultLifetime = 5;
layout(location = 3)uniform vec3 emitterPosition = vec3(0,0,0);

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand(uint seed, float range)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
    oPosition = iPosition + iVelocity * deltaTime;
    oVelocity = iVelocity;
    oLifetime = iLifetime - deltaTime;

    // should emit?
    if(oLifetime < 0.0f)
    {
        uint rSeed = uint(time * 1000.0f) + uint(gl_VertexID);

        oVelocity.x = rand(rSeed++, 2) - 1;
        oVelocity.y = rand(rSeed++, 2) - 1;
        oVelocity.z = rand(rSeed++, 2) - 1;
        oVelocity = normalize(oVelocity);

        oPosition = emitterPosition;
        oLifetime = defaultLifetime;
    }
}