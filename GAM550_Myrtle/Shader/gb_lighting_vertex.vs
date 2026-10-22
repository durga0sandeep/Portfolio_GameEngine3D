/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 430

//uniform mat4 WorldView, WorldInverse, WorldProj, ModelTr, NormalTr, ShadowMatrix;

in vec4 vertex;
in vec2 vertexTexture;////06-04-2025
out vec3 tanVec;
out vec2 texCoord;


void main()
{      
	
	texCoord = vertexTexture;//06-04-2025

	gl_Position = vertex;

}

