#version 430

out vec4 FragData[4];

in vec3 normalVec;
in vec2 texCoord;
in vec3 worldPos;

uniform sampler2D mainTexture;
uniform vec3 specular;
uniform float shininess;

//NORMAL MAP NEEDS THEM - not now
//in vec3 tanVec;  // Tangent vector for normal mapping

void main()
{
    //NORMAL MAP NEEDS THEM - not now
    /*
    vec3 T = normalize(tanVec); //NORMAL MAP
    vec3 B = normalize(cross(T, N));  // Binormal
    vec3 delta = texture(normalMap, uv).xyz;
    delta = (delta * 2.0) - vec3(1.0, 1.0, 1.0);  // Convert from [0, 1] to [-1, 1]
    N = (delta.x * T) + (delta.y * B) + (delta.z * N);  // Recalculate normal
    */

    vec3 N = normalize(normalVec);
    //vec3 Kd = texture(mainTexture, texCoord).rgb;
    vec3 Kd = texture(mainTexture, vec2(texCoord.x, 1.0 - texCoord.y)).rgb;
    vec3 Ks = specular;
    float Alpha = shininess;


    FragData[0] = vec4(worldPos, 1.0);   // Position
    FragData[1] = vec4(N, 0.0);          // Normal
    FragData[2] = vec4(Kd, 1.0);         // Diffuse color
    FragData[3] = vec4(Ks, Alpha);       // Specular color + shininess
}





