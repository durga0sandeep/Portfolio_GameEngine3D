/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texcoord;


void main()
{      
	
	texcoord = aTexCoord;//06-04-2025

	gl_Position = vec4(aPos,1);

}

