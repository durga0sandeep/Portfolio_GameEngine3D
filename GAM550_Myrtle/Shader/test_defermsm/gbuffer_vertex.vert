/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting (G-Buffer)
/////////////////////////////////////////////////////////////////////////
#version 430

// Vertex attributes
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vertexTangent; // Added location 3 explicitly

// Uniforms
uniform mat4 WorldView;
//uniform mat4 WorldInverse;
uniform mat4 WorldProj;
uniform mat4 ModelTr;
uniform mat4 NormalTr;
//uniform mat4 ShadowMatrix;
//uniform vec3 lightPos;

// Outputs to fragment shader
out vec3 tanVec;
out vec3 normalVec;
//out vec3 lightVec;
//out vec3 eyeVec;
out vec2 texCoord;
//out vec4 ShadowCoord;
out vec3 worldPos;

void main()
{
    // Transform tangent to world space
    tanVec = mat3(ModelTr) * vertexTangent;

    // Transform position to clip space
    gl_Position = WorldProj * WorldView * ModelTr * vec4(vPos, 1.0);

    // Shadow map coordinates
    // ShadowCoord = ShadowMatrix * ModelTr * vec4(vPos, 1.0);

    // World position
    worldPos = (ModelTr * vec4(vPos, 1.0)).xyz;

    // Transform normal to world space
    normalVec = mat3(NormalTr) * normal;

    // Eye and light vectors in world space
    //vec3 eyePos = (WorldInverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    //lightVec = lightPos - worldPos;
    //eyeVec = eyePos - worldPos;

    // Pass UV coordinates
    texCoord = vTexCoord;
}
