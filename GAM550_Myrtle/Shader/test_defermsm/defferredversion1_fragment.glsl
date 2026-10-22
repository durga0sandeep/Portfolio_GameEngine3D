#version 430 core

in vec2 texcoord;

layout(location = 0) out vec4 FragColor;

// ---- G-buffer ----
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;      // Diffuse color (Kd) + alpha
uniform sampler2D gMaterial;    // Specular color (Ks) + shininess

// Lighting / camera
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;

// Shadow map
uniform sampler2DShadow shadowMap;
uniform mat4 lightSpaceMatrix;

// Material properties (use uniforms like forward pass)
uniform float metallic;
uniform float roughness;

const float PI = 3.14159265359;

// ========================
//        PBR Helpers
// ========================

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
    return NdotV / (NdotV * (1.0 - k) + k);
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


// ========================
//     Shadow Calculation
// ========================

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 ||
        projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 1.0;
    }

    return texture(shadowMap, projCoords);
}


// ========================
//          MAIN
// ========================

void main()
{
    // Read from G-buffer
    vec3 FragPos = texture(gPosition, texcoord).xyz;
    vec3 N = normalize(texture(gNormal, texcoord).xyz);

    // gAlbedo contains: Kd (diffuse RGB) + alpha
    vec4 albedoData = texture(gAlbedo, texcoord);
    vec3 albedo = albedoData.rgb;
    float alpha = albedoData.a;

    // gMaterial contains: Ks (specular RGB) + shininess
    // We're not using these in PBR, but they're available if needed
    // vec4 materialData = texture(gMaterial, texcoord);
    // vec3 specularColor = materialData.rgb;
    // float shininess = materialData.a;

    // Early discard for fully transparent pixels
    if(alpha < 0.01)
        discard;

    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);

    // Fresnel (use uniform metallic instead of reading from texture)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Use uniform roughness instead of reading from texture
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denom = 4.0 * max(dot(N,V), 0.0) * max(dot(N,L), 0.0) + 0.001;
    vec3 specular = numerator / denom;

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    vec3 Lo = (kD * albedo / PI + specular) * lightColor * NdotL;

    // Shadow calculation
    float shadow = ShadowCalculation(lightSpaceMatrix * vec4(FragPos, 1.0));

    // Ambient (no AO stored in G-buffer, so use 1.0)
    vec3 ambient = ambientLight * albedo;

    // Final lighting
    vec3 color = ambient + Lo * shadow;

    // tone-mapping + gamma (remove these if you want to match forward pass)
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, alpha);
}