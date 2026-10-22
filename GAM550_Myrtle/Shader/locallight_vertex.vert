/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 430

in vec4 vertex;
uniform mat4 WorldView, WorldProj, WorldInverse;
out vec3 eyeVec;

struct PointLight {
        vec3 position;
        vec3 color;
        float intensity;
        float radius;
	int padding[2];
};

const int MAX_POINT_LIGHTS = 25;//100;//1000;//4

layout(std430, binding = 1) buffer LightBuffer{
	PointLight pointlights[MAX_POINT_LIGHTS];
};

uniform int lightIndex;


void main()
{      

    vec4 worldPos = vec4((vertex.xyz * pointlights[lightIndex].radius) + pointlights[lightIndex].position, 1.0f);

    vec3 eyePos = (WorldInverse * vec4(0,0,0,1)).xyz;
    eyeVec = eyePos - worldPos.xyz;

    gl_Position = WorldProj * WorldView * worldPos;
	
}





