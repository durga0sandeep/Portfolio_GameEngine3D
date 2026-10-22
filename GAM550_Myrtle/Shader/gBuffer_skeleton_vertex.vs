/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

#define MAX_BONES 200
#define MAX_BONE_INFLUENCE 4
uniform mat4 gBones[MAX_BONES];
uniform mat4 ModelMatrix,ViewMatrix, ProjectionMatrix,InverseNormalMatrix;

uniform bool UseSkinning;

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;
layout(location = 3) in vec3 vertexTangent;

layout(location = 4) in ivec4 boneIDs;     // Bone indices
layout(location = 5) in vec4 boneWeights;  // Bone weights

out vec3 normalVec;
out vec2 texCoord;
out vec4 worldPos;

void main()
{
    // --- Skinning ---
    vec4 finalVertex;
    vec3 finalNormal;

    if (UseSkinning) {
        // --- Skinning ---
        vec4 skinnedVertex = vec4(0.0);
        vec3 skinnedNormal = vec3(0.0);

        for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            int id = boneIDs[i];
            float w = boneWeights[i];

            if (id == -1 || w == 0.0) 
                continue;

            mat4 boneTransform = gBones[id];
            skinnedVertex += (boneTransform * vertex) * w;
            skinnedNormal += (mat3(boneTransform) * vertexNormal) * w;
        }

        finalVertex = skinnedVertex;
        finalNormal = skinnedNormal;
    }

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * finalVertex;
    worldPos = ModelMatrix * finalVertex;
    normalVec = vec3(normalize(InverseNormalMatrix * vec4(finalNormal,1.0)));
    texCoord = vertexTexture;
}
