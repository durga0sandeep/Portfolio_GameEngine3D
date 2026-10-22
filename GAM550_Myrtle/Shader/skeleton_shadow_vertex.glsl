#version 330

#define MAX_BONES 200
#define MAX_BONE_INFLUENCE 4

layout(location = 0) in vec3 aPos;
uniform mat4 lightSpaceMatrix;

uniform mat4 gBones[MAX_BONES];
uniform mat4 ModelMatrix,ViewMatrix, ProjectionMatrix;
uniform vec3 Color;
uniform bool UseSkinning;

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;
layout(location = 3) in vec3 vertexTangent;

layout(location = 4) in ivec4 boneIDs;
layout(location = 5) in vec4 boneWeights;

//out vec3 final_color;
//out vec3 Normal;
//out vec2 texcoord;
//out vec3 FragPos;

void main()
{
    vec4 skinnedVertex = vec4(0.0);
    vec3 skinnedNormal = vec3(0.0);

    if (UseSkinning) {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            int id = boneIDs[i];
            float w = boneWeights[i];

            if (id < 0 || id >= MAX_BONES || w == 0.0)
                continue;

            mat4 boneTransform = gBones[id];
            skinnedVertex += (boneTransform * vertex) * w;
            skinnedNormal += (mat3(boneTransform) * vertexNormal) * w;
        }
    } else {
        skinnedVertex = vertex;
        skinnedNormal = vertexNormal;
    }

    //final_color = Color;

    //gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * skinnedVertex;
    gl_Position = lightSpaceMatrix * ModelMatrix * skinnedVertex;


    //FragPos = vec3(skinnedVertex);
    //Normal = normalize(skinnedNormal);
    //texcoord = vertexTexture;
}
