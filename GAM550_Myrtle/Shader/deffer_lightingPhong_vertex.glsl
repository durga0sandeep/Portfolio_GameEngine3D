#version 330 core

layout(location = 0) in vec2 aPos;      // Fullscreen quad vertex position
layout(location = 1) in vec2 aTexCoord; // Texture coordinate

out vec2 texcoord;

void main()
{
    texcoord = aTexCoord;
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}
