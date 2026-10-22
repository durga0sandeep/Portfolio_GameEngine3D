#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vTexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 viewPos;
out vec3 eyeVec;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPos, 1.0);
	vec3 worldPos = (ModelMatrix*vec4(vPos,1)).xyz;
	eyeVec =viewPos-worldPos;
}
