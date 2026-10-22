/////////////////////////////////////////////////////////////////////////
// Vertex shader for SHADOW
////////////////////////////////////////////////////////////////////////
#version 430

uniform mat4 model;
uniform mat4 lightSpaceMatrix;


in vec4 vertex;

out vec4 position;

void main()
{      
	gl_Position = lightSpaceMatrix * model * vertex;
	position = gl_Position;
}

