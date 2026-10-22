#version 430 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 WorldView;
uniform mat4 WorldProj;


void main()
{
    gl_Position = WorldProj * WorldView * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
