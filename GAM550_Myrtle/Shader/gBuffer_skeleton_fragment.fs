#version 330 core

out vec4 FragData[4];

in vec3 normalVec;
in vec2 texCoord;
in vec4 worldPos;

uniform sampler2D mainTexture;
uniform vec3 specular;
uniform float shininess;

void main()
{   	
	vec3 N = normalize(normalVec);
	vec3 Kd = texture(mainTexture, texCoord).rgb;
	vec3 Ks = specular;
	float Alpha = shininess;


	FragData[0] = worldPos;              // Position
	FragData[1] = vec4(N, 0.0);          // Normal
	FragData[2] = vec4(Kd, 1.0);         // Diffuse color
	FragData[3] = vec4(Ks, Alpha);       // Specular color + shininess

}