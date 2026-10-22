/////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting (G-Buffer)
/////////////////////////////////////////////////////////////////////////
#version 430

out vec4 FragData[4]; // G-Buffer outputs

const int groundId = 3;

// Inputs from vertex shader
in vec3 normalVec;
//in vec3 lightVec;
//in vec3 eyeVec;
in vec2 texCoord;
in vec3 worldPos;
in vec3 tanVec;

// Uniforms
uniform sampler2D tex;
uniform sampler2D normalMap;

uniform int objectId;
//uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
//uniform vec3 Light;
uniform vec3 Ambient;

void main()
{
    // Normalize normal vector
    vec3 N = normalize(normalVec);

    // Material properties
    //vec3 Kd = diffuse;
    vec3 Kd = vec3(0.3f, 0.2f, 0.1f);
    vec3 Ks = specular;
    float Alpha = shininess;

    // Texture coordinates
    vec2 uv = texCoord;

    // Apply ground-specific texture
    if (objectId == groundId)
    {
        uv = fract(texCoord * 120.0);
        vec4 color = texture(tex, uv);
        Kd = color.rgb;
    }
    else
    {
        uv = fract(texCoord * 120.0);
        vec4 color = texture(tex, uv);
        Kd = color.rgb;

        // Optional normal map (commented out)
        /*
        vec3 T = normalize(tanVec);
        vec3 B = normalize(cross(T, N));
        vec3 delta = texture(normalMap, uv).rgb * 2.0 - vec3(1.0);
        N = normalize(delta.x * T + delta.y * B + delta.z * N);
        */
    }

    // Write G-Buffer data
    FragData[0].xyz = worldPos;    // World position
    FragData[1].xyz = N;           // Normal
    FragData[2].xyz = Kd;          // Diffuse color
    FragData[3] = vec4(Ks, Alpha); // Specular + shininess

}
