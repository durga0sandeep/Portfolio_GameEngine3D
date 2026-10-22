/////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting (fixed + GGX)
////////////////////////////////////////////////////////////////////////
#version 430

// Textures
uniform sampler2D WorldPosMap;
uniform sampler2D NMap;  // normal map stored in [0,1] (world-space or T-space depending on your pipeline)
uniform sampler2D KdMap; // albedo
uniform sampler2D KaMap; // we use .rgb for specular color, .a for roughness (0..1)

out vec4 fragColor;        // Output color of the fragment

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
    int padding[2];
};

const int MAX_POINT_LIGHTS = 25;

layout(std430, binding = 1) buffer LightBuffer {
    PointLight pointlights[MAX_POINT_LIGHTS];
};

uniform int lightIndex;
uniform vec3 viewPos;      // camera position in world space
uniform vec2 ScreenSize;   // pass viewport size here (e.g. vec2(width, height))

in vec3 eyeVec; // if provided (but we compute V from viewPos as fallback)

const float PI = 3.14159265359;

// Schlick Fresnel
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX / Trowbridge-Reitz normal distribution
float DistributionGGX(float NdotH, float a2) {
    float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
    return a2 / max(denom, 1e-6);
}

// Schlick-GGX Geometry (Smith)
float GeometrySchlickGGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float NdotV, float NdotL, float k) {
    return GeometrySchlickGGX(NdotV, k) * GeometrySchlickGGX(NdotL, k);
}

void main() {
    fragColor = vec4(0.0);

    vec2 uv = gl_FragCoord.xy / ScreenSize;
    // fetch G-buffer values
    vec3 fragPosition = texture(WorldPosMap, uv).xyz;
    vec3 N_tex = texture(NMap, uv).rgb;
    // decode normal from [0,1] to [-1,1] and normalize.
    vec3 N = normalize(N_tex * 2.0 - 1.0);

    // material maps
    vec3 Kd = texture(KdMap, uv).rgb;     // diffuse / albedo
    vec3 Ks = texture(KaMap, uv).rgb;     // specular color (F0)
    float roughness = texture(KaMap, uv).a; // roughness in [0..1]
    roughness = clamp(roughness, 0.04, 1.0); // avoid 0 (perfect mirror) numerical issues
    float a2 = roughness * roughness; // squared roughness for GGX

    // light
    PointLight Ldata = pointlights[lightIndex];
    vec3 Lpos = Ldata.position;
    vec3 radiance = Ldata.color * Ldata.intensity;

    float dist = length(Lpos - fragPosition);
    if (dist > Ldata.radius) {
        discard;
    }

    // stable attenuation: smooth falloff inside radius, avoid singularities
    float radius = max(0.0001, Ldata.radius);
    float linearFalloff = max(0.0, 1.0 - (dist / radius)); // 1 at center, 0 at edge
    float attenuation = linearFalloff / (dist * dist + 1.0);

    vec3 L = normalize(Lpos - fragPosition);
    vec3 V = normalize(viewPos - fragPosition); // use viewPos (more robust than eyeVec)
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0001);
    float NdotH = max(dot(N, H), 0.0001);
    float LdotH = max(dot(L, H), 0.0);

    // -- BRDF terms (GGX)
    float D = DistributionGGX(NdotH, a2);
    vec3 F = fresnelSchlick(LdotH, Ks); // Ks used as F0
    // Schlick-GGX geometry factor parameter k from roughness
    float k = (roughness + 1.0);
    k = (k*k) / 8.0; // UE4 style remap
    float G = GeometrySmith(NdotV, NdotL, k);

    vec3 specular = (D * G * F) / max(4.0 * NdotV * NdotL, 1e-6);

    // diffuse (Lambert) multiplied by energy-conserving factor (1 - F)
    vec3 diffuse = (1.0 - F) * (Kd / PI);

    // accumulate
    vec3 Lo = (diffuse + specular) * radiance * NdotL * attenuation;

    fragColor.rgb = Lo;
    fragColor.a = 1.0;

    // optional: gamma-correct output here (if you want)
    // fragColor.rgb = pow(fragColor.rgb, vec3(1.0/2.2));
}
