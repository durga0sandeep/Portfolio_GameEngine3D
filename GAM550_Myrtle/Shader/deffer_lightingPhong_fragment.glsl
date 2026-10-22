#version 330 core
out vec4 FragColor;

in vec2 texcoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMaterial;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;

const float PI = 3.14159265359;


// GGX / Trowbridge-Reitz normal distribution function
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}


// Geometry function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}


float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}


// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 FragPos = texture(gPosition, texcoord).xyz;
    vec3 Normal = normalize(texture(gNormal, texcoord).xyz);
    vec3 albedo = texture(gAlbedo, texcoord).rgb;
    vec4 material = texture(gMaterial, texcoord);
    vec3 specularColor = material.rgb;  // Specular color from G-buffer
    float shininess = material.a;       // Shininess from G-buffer
    
    // Blinn-Phong lighting
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);
    
    // Diffuse
    float NdotL = max(dot(Normal, L), 0.0);
    vec3 diffuse = albedo * lightColor * NdotL;
    
    // Specular (Blinn-Phong)
    float NdotH = max(dot(Normal, H), 0.0);
    vec3 specular = specularColor * lightColor * pow(NdotH, shininess);
    
    // Ambient
    vec3 ambient = ambientLight * albedo;
    
    // Final color
    vec3 color = ambient + diffuse + specular;
    
    FragColor = vec4(color, 1.0);
}
