#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D particleTexture;

void main()
{
    vec4 texColor = texture(particleTexture, vUV);
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}