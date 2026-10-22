/////////////////////////////////////////////////////////////////////////
// Universal Pixel Shader (no objectId)
/////////////////////////////////////////////////////////////////////////
#version 330

out vec4 FragColor;

uniform sampler2D tex;           // Base color (diffuse/albedo) texture
uniform sampler2D normalMap;     // Normal map (optional)
uniform sampler2D shadowMap;     // Shadow map
uniform sampler2D BlurredShadowMap;

uniform sampler2D WorldPosMap;   // From G-buffer
uniform sampler2D NMap;          // From G-buffer
uniform sampler2D KdMap;         // Diffuse coefficient
uniform sampler2D KaMap;         // Specular coefficient (Ks) and alpha

uniform mat4 WorldInverse;
uniform mat4 ShadowMatrix;

uniform vec3 lightPos;          // Light position (in world space)
uniform vec3 Light;              // Light color/intensity
uniform vec3 Ambient;            // Ambient light
uniform float shininess;

in vec3 tanVec;
in vec2 texCoord;

const float Pi = 3.14159265;

vec2 uv;

uniform sampler2D mainTexture;



// === Function to apply normal mapping if available ===
vec3 applyNormalMap(vec3 N, vec3 T, sampler2D nMap, vec2 uv)
{
    vec3 normalTex = texture(nMap, uv).xyz * 2.0 - 1.0;
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * normalTex);
}

// === Main ===
void main()
{
 
     //TESTING    
     uv = gl_FragCoord.xy/vec2(750,750); // Or use textureSize
     vec3 pos = texture(KdMap, uv).xyz;

     // Visualize the position (scale/bias might be needed to see colors)
     FragColor = vec4(abs(pos) * 0.1, 1.0); // Example visualization
     FragColor = vec4(0.0, 0.0, 1.0, 1.0); // blue color
     return;
     

    // Compute texture coordinates (screen-space lookup)
    uv = gl_FragCoord.xy / vec2(750.0, 750.0);

    // World-space position and normal from G-buffer
    vec3 WorldPos = texture(WorldPosMap, uv).xyz;
    vec3 N = normalize(texture(NMap, uv).xyz);

    // Base color (diffuse/albedo)
    vec3 Kd = texture(tex, texCoord).xyz;

    // Specular and alpha (shininess)
    vec3 Ks = texture(KaMap, uv).xyz;
    float Alpha = max(texture(KaMap, uv).a * 100.0, 1.0);

    // Optionally apply normal map
    N = applyNormalMap(N, normalize(tanVec), normalMap, texCoord);

    // Compute lighting vectors
    vec3 eyePos = (WorldInverse * vec4(0, 0, 0, 1)).xyz;
    vec3 L = normalize(lightPos - WorldPos);
    vec3 V = normalize(eyePos - WorldPos);
    vec3 H = normalize(L + V);

    
    // === Shadow Calculation ===
    vec4 ShadowCoord = ShadowMatrix * vec4(WorldPos, 1.0);
    vec2 shadowUV = ShadowCoord.xy / ShadowCoord.w;
    bool inShadow = false;

    if (shadowUV.x >= 0.0 && shadowUV.x <= 1.0 &&
        shadowUV.y >= 0.0 && shadowUV.y <= 1.0 &&
        ShadowCoord.w > 0.0)
    {
        float lightDepth = texture(shadowMap, shadowUV).r;
        float pixelDepth = ShadowCoord.z / ShadowCoord.w;
        inShadow = (pixelDepth - 0.002 > lightDepth);
    }
    

    // === BRDF Lighting ===
    float LN = max(dot(L, N), 0.0);
    float HN = max(dot(H, N), 0.0);

    float D = ((Alpha + 2.0) / (2.0 * Pi)) * pow(HN, Alpha);
    vec3 F = Ks + (vec3(1.0) - Ks) * pow((1.0 - max(dot(H, L), 0.0)), 5.0);
    float G = 1.0 / pow(max(dot(H, L), 0.0001), 2.0);

    vec3 BRDF = (Kd / Pi) + (F * G * D) / 4.0;

    vec3 color;
    if(inShadow)
    {
        color = Ambient * Kd; // Only ambient in shadow
    }
    else{
        color = Ambient * Kd + Light * LN * BRDF;
    }
    
    FragColor = vec4(color, 1.0);


    //FragColor = vec4(0.0, 0.0, 1.0, 1.0); // blue color

}

