#version 330

out vec4 FragColor;

in vec3 eyeVec;
uniform sampler2D mainTexture;
#define PI 3.14159265359

void main()
{
    vec3 V = normalize(eyeVec);
    V.z = clamp(V.z, -0.99999, 0.99999);
    //if(V.z >0) V.z = -V.z;
    //vec2 uv = vec2(-atan(V.y,V.x)/(2*PI), acos(V.z)/PI);
    vec2 uv = vec2(atan(V.x, V.z) / (2.0 * PI), acos(V.y) / PI);
    uv = fract(uv);
    vec4 texColor = texture(mainTexture, uv);
    FragColor = vec4(texColor.xyz, 1.0);
    //FragColor = vec4(1.0,0.0,0.0, 1.0);
}