#version 430 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightRadius;
uniform float lightIntensity;
uniform vec3 cameraPos;

uniform vec2 screenSize;

void main()
{
    vec2 uv = gl_FragCoord.xy / screenSize;
    vec3 worldPos = texture(gPosition, uv).rgb;
    vec3 normal   = normalize(texture(gNormal, uv).rgb);
    vec3 albedo   = texture(gAlbedo, uv).rgb;

    vec3 L = lightPos - worldPos;
    float dist = length(L);
    L = normalize(L);

    float NdotL = max(dot(normal, L), 0.0);

    // Quadratic attenuation
    float attenuation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

    // Diffuse
    vec3 diffuse = albedo * lightColor * NdotL * attenuation * lightIntensity;

    // Optional specular (Blinn-Phong)
    vec3 V = normalize(cameraPos - worldPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(normal, H), 0.0), 32.0);
    vec3 specular = lightColor * spec * attenuation * lightIntensity;

    FragColor = vec4(diffuse + specular, 1.0);
}