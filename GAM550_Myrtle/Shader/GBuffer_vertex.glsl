#version 430

uniform mat4 WorldView;
uniform mat4 WorldProj;
uniform mat4 ModelTr;
uniform mat4 NormalTr;

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

out vec3 normalVec;
out vec2 texCoord;
out vec3 worldPos;

//NORMAL MAP NEEDS THEM - not now
//in vec3 vertexTangent;
//out vec3 tanVec;

void main()
{
    //NORMAL MAP NEEDS THEM -  not now
    //// Transform tangent vector to world space
    //tanVec = mat3(ModelTr) * vertexTangent;

    gl_Position = WorldProj * WorldView * ModelTr * vertex;
    worldPos = (ModelTr * vertex).xyz;
    normalVec = mat3(NormalTr) * vertexNormal;
    texCoord = vertexTexture;
}




