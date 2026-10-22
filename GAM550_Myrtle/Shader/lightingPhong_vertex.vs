#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 2) in vec2 vTexCoord;
layout(location = 1) in vec3 normal;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 Color;

out vec3 final_color;
out vec2 texcoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
    final_color = Color;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPos, 1.0);
    texcoord = vTexCoord;
    Normal = mat3(transpose(inverse(ModelMatrix))) * normal;
    FragPos = vec3(ModelMatrix * vec4(vPos, 1.0));
}

