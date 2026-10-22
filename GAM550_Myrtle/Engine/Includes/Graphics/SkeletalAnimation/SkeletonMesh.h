/*!****************************************************************************
 * file  : SkeletonMesh.h
 * brief : Skeleton Mesh class to load mesh with bones and weights
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once

#include <map>
#include <vector>

#include "mySkeleton.h"

#include <Graphics/Texture.h>
#include <Graphics/Material.h>
#include "Graphics/Mesh.h"

#define MAX_NUM_BONES_PER_VERTEX 4

//Bone Ids and weights (up to 4)
struct BoneData
{
    int ids[MAX_NUM_BONES_PER_VERTEX] = { 0 };
    float w[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };

    BoneData()
    {
    }

    void AddBoneData(int _id, float _w)
    {
        int arr_size = sizeof(ids) / sizeof(ids[0]);
        for (int i = 0; i < arr_size; i++) {
            if (w[i] == 0.0) {
                ids[i] = _id;
                w[i] = _w;
                return;
            }
        }

        assert(0);
    }
};


class SkeletonMesh : public Mesh
{
public:
    SkeletonMesh();
	~SkeletonMesh();

    void LoadMesh(const std::string& path);
	void Draw();
    void MakeVAO();
    void setObjectTransform(const glm::mat4& transform) { objectTr = transform; }
    void setMaterial(std::shared_ptr<Material> material) { m_material = material; }
    void setShadowMaterial(std::shared_ptr<Material> material) { m_material = material; }

    void DrawShadow(std::shared_ptr<Material> mat);

    std::shared_ptr<mySkeleton> m_skeleton = nullptr;
private:

#define MAX_NUM_BONES_PER_VERTEX 4

    void InitMesh(const std::string& path);
    void InitBone(const aiMesh* mesh, int index);
    int GetBoneId(const aiBone* b);
    

    GLuint m_vao = 0;

	Assimp::Importer Importer;
	const aiScene* m_scene = NULL;

	std::vector<glm::vec4> m_pnt;
	std::vector<glm::vec3> m_norm;
	std::vector<glm::vec2> m_tex;
	std::vector<glm::vec3> m_tan;

	// vertex indices for each triangle
	std::vector<int> m_indices;
	//each vertex bone ids and weights
    std::vector<BoneData> m_bones;
    
	// texture for animation, just the colors of each vertex
    std::vector<std::shared_ptr<Texture>> m_diffuse;
    std::shared_ptr<Material> m_material;
    std::shared_ptr<Material> m_shadow_material;

    glm::mat4 objectTr = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -2.5f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    aiMatrix4x4 m_inverseTrans;
	// bone name and index map
    std::map<std::string, int> m_name_index;

};
