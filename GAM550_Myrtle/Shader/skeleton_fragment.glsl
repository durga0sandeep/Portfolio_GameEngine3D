#version 330 core
out vec4 FragColor;

in vec2 texcoord;
in vec3 final_color;
in vec3 FragPos;
in vec3 Normal;

uniform bool useTexture;
uniform bool isTransparent;
uniform sampler2D mainTexture;
uniform sampler2DShadow shadowMap;
uniform mat4 lightSpaceMatrix;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float metallic;
uniform float roughness;

uniform vec3 ambientLight;

uniform vec4 uColor;
uniform float fadeFactor;

const float PI = 3.14159265359;

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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // Check if outside shadow map bounds (fully lit)
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || 
        projCoords.y < 0.0 || projCoords.y > 1.0)
        return 1.0;
    
    float shadow = texture(shadowMap, projCoords);
    return shadow;
}

void main() {
    // Get base color
    vec4 baseColor;
    if (useTexture)
        baseColor = texture(mainTexture, fract(texcoord));
    else {
        if (isTransparent)
            baseColor = vec4(final_color, 0.0);
        else
            baseColor = vec4(final_color, 1.0);
    }
    
    vec4 albedo = uColor * baseColor;
    
    // PBR calculations
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);
    
    vec4 F04 = vec4(0.04, 0.04, 0.04, 1.0); 
    F04 = mix(F04, albedo, metallic);
    vec3 F0 = vec3(F04); 
    
    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0); 
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;     
    
    float NdotL = max(dot(N, L), 0.0);        
    vec3 Lo = (kD * vec3(albedo) / PI + specular) * lightColor * NdotL;
    
    // Shadow calculation
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = ShadowCalculation(fragPosLightSpace);
    
    // Apply shadow only to direct lighting
    vec3 ambient = ambientLight * vec3(albedo);
    vec3 lighting = ambient + Lo * shadow;
    
    FragColor = vec4(lighting * fadeFactor, albedo.a);
}
