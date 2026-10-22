#include "pch.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Graphics/SkeletalAnimation/SkeletonMesh.h"
//#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 convert(const aiMatrix4x4& m)
{
	glm::mat4 res;

	res[0][0] = m.a1; res[0][1] = m.b1; res[0][2] = m.c1; res[0][3] = m.d1;
	res[1][0] = m.a2; res[1][1] = m.b2; res[1][2] = m.c2; res[1][3] = m.d2;
	res[2][0] = m.a3; res[2][1] = m.b3; res[2][2] = m.c3; res[2][3] = m.d3;
	res[3][0] = m.a4; res[3][1] = m.b4; res[3][2] = m.c4; res[3][3] = m.d4;
	return res;
}

glm::mat4 convert(const aiMatrix3x3& m)
{
	glm::mat4 res;
	res[0][0] = m.a1; res[0][1] = m.a2; res[0][2] = m.a3; res[0][3] = 0.0;
	res[1][0] = m.b1; res[1][1] = m.b2; res[1][2] = m.b3; res[1][3] = 0.0;
	res[2][0] = m.c1; res[2][1] = m.c2; res[2][2] = m.c3; res[2][3] = 0.0;
	res[3][0] = 0.0; res[3][1] =0.0; res[3][2] = 0.0; res[3][3] = 1.0;
	return res;
}



SkeletonMesh::SkeletonMesh()
{
}

SkeletonMesh::~SkeletonMesh()
{
}

void SkeletonMesh::LoadMesh(const std::string& path)
{
	Importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	m_scene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate 
											| aiProcess_GenSmoothNormals 
											| aiProcess_FlipUVs 
											| aiProcess_JoinIdenticalVertices);
	if (!m_scene)
	{
		printf("Error parsing '%s': '%s'\n", path.c_str(), Importer.GetErrorString());
		return;
	}

	
	m_inverseTrans = m_scene->mRootNode->mTransformation;
	m_inverseTrans = m_inverseTrans.Inverse();

	InitMesh(path);

	MakeVAO();
    setReady();
}

/**
 * $BRIEF
 * Skeleton Mesh Draw with skinning helper function, call in renderer
 */
void SkeletonMesh::Draw()
{
	//glm::vec3 diffuseColor{0.7,0.7,0.5};
	glm::vec3 specularColor{0.5,0.5,0.5};
	float shininess = 1.0f;

	//CHECKERROR;
	glBindVertexArray(m_vao);
	//CHECKERROR;

	for (int i = 0; i < m_skeleton->m_boneInfo.size(); ++i)
	{
		std::string name = "gBones[" + std::to_string(i) + "]";
        
		//int loc = glGetUniformLocation(programId, name.c_str());

		//glUniformMatrix4fv(loc, 1, GL_TRUE, &glm::mat4(1.0)[0][0]);
		auto info = m_skeleton->m_boneInfo[i];

		glm::mat4 boneTransform = info.finalTransformation;

		//glUniformMatrix4fv(loc, 1, GL_FALSE, &boneTransform[0][0]);
        m_material->setProperty(name, boneTransform);
		//CHECKERROR;
	}

	int vertices = 0;
	int indices = 0;
	int staticcount = 0;
	for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i)
	{
		int matIndex = m_scene->mMeshes[i]->mMaterialIndex;
        m_material->setTexture("mainTexture",m_diffuse[matIndex],0);
        

		//int loc;
		//= glGetUniformLocation(programId, "diffuse");
		//glUniform3fv(loc, 1, &diffuseColor[0]);
		if(m_scene->mMeshes[i]->HasBones())
		{
			//glUniform1i(glGetUniformLocation(programId, "UseSkinning"), GL_TRUE);
            m_material->setProperty("UseSkinning", true);
		}

		//glm::mat4 inv = glm::inverse(objectTr);
		////loc = glGetUniformLocation(programId, "NormalTr");
		////glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(inv));

  //      m_material->setProperty("NormalTr", inv);
        m_material->apply();
		int curr_indices = m_scene->mMeshes[i]->mNumFaces * 3;
		glDrawElementsBaseVertex(GL_TRIANGLES, curr_indices, GL_UNSIGNED_INT,
								(void*)(sizeof(unsigned int) * indices), vertices);

		vertices += m_scene->mMeshes[i]->mNumVertices;
		indices += curr_indices;
		}
	//CHECKERROR;
	glBindVertexArray(0);
}


void SkeletonMesh::DrawShadow(std::shared_ptr<Material> mat)
{
    //glm::vec3 diffuseColor{0.7,0.7,0.5};
    glm::vec3 specularColor{ 0.5,0.5,0.5 };
    float shininess = 1.0f;

    //CHECKERROR;
    glBindVertexArray(m_vao);
    //CHECKERROR;
    //std::cout << m_skeleton->m_boneInfo.size() << std::endl;
    for (int i = 0; i < m_skeleton->m_boneInfo.size(); ++i)
    {
        std::string name = "gBones[" + std::to_string(i) + "]";

        auto info = m_skeleton->m_boneInfo[i];

        glm::mat4 boneTransform = info.finalTransformation;

        mat->setProperty(name, boneTransform);
        //std::cout << " BoneTransform " << name << " " << std::endl;
        //CHECKERROR;
    }

    int vertices = 0;
    int indices = 0;
    int staticcount = 0;
    for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i)
    {
        int matIndex = m_scene->mMeshes[i]->mMaterialIndex;
        //mat->setTexture("mainTexture", m_diffuse[matIndex], 0);
        mat->setProperty("UseSkinning", true);
        mat->apply();

        int curr_indices = m_scene->mMeshes[i]->mNumFaces * 3;
        glDrawElementsBaseVertex(GL_TRIANGLES, curr_indices, GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * indices), vertices);

        vertices += m_scene->mMeshes[i]->mNumVertices;
        indices += curr_indices;
    }
    //CHECKERROR;
    glBindVertexArray(0);
}


/**
 * $BRIEF
 * Skeleton Mesh need a special VAO to store bone data, so I add this function to create it
 */
void SkeletonMesh::MakeVAO()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	GLuint Pbuff;
	glGenBuffers(1, &Pbuff);
	glBindBuffer(GL_ARRAY_BUFFER, Pbuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * m_pnt.size(),
		&m_pnt[0][0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (m_norm.size() > 0) {
		GLuint Nbuff;
		glGenBuffers(1, &Nbuff);
		glBindBuffer(GL_ARRAY_BUFFER, Nbuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_norm.size(),
			&m_norm[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_tex.size() > 0) {
		GLuint Tbuff;
		glGenBuffers(1, &Tbuff);
		glBindBuffer(GL_ARRAY_BUFFER, Tbuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m_tex.size(),
			&m_tex[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_tan.size() > 0) {
		GLuint Dbuff;
		glGenBuffers(1, &Dbuff);
		glBindBuffer(GL_ARRAY_BUFFER, Dbuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_tan.size(),
			&m_tex[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint Bbuff;
	glGenBuffers(1, &Bbuff);
	glBindBuffer(GL_ARRAY_BUFFER, Bbuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoneData) * m_bones.size(),&m_bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(BoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT,GL_FALSE, sizeof(BoneData),(const GLvoid*)(MAX_NUM_BONES_PER_VERTEX *sizeof(int)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint Ibuff;
	glGenBuffers(1, &Ibuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* m_indices.size(),
		&m_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

std::string ExtractFilename(const std::string& fullPath) {
	size_t pos = fullPath.find_last_of("/\\");
	return (pos != std::string::npos) ? fullPath.substr(pos + 1) : fullPath;
}

void SkeletonMesh::InitMesh(const std::string& path)
{
	int num_vertices = 0;
	int staticMeshIndex = 0;
	//load meshes
	for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i)
	{
		const aiMesh* m = m_scene->mMeshes[i];

		aiMaterial* material = m_scene->mMaterials[m->mMaterialIndex];
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		std::string rawPath = ExtractFilename(path.C_Str());
		std::string fixedPath = "Model/textures/" + rawPath;

		std::cout << "texture path: " << fixedPath << std::endl;
		m_diffuse.push_back(Texture::create(fixedPath, Texture::Type::Texture2D,false));
        m_diffuse.back()->setFilterMode(Texture::FilterMode::Linear);

		//individual mesh
		for (unsigned int j = 0; j < m->mNumVertices; ++j)
		{
			aiVector3D& p = m->mVertices[j];
			m_pnt.push_back(glm::vec4(p.x, p.y, p.z, 1.0));

			if (m->mNormals)
			{
				aiVector3D& n = m->mNormals[j];
				m_norm.push_back(glm::vec3(n.x, n.y, n.z));
			}

			if (m->mTangents)
			{
				aiVector3D& tan = m->mTangents[j];
				m_tan.push_back(glm::vec3(tan.x, tan.y, tan.z));
			}

			const aiVector3D& tx = m->HasTextureCoords(0) ? m->mTextureCoords[0][j] : aiVector3D(0.0f, 0.0f, 0.0f);
			m_tex.push_back(glm::vec2(tx.x, tx.y));
			m_bones.push_back(BoneData());
		}

		InitBone(m, num_vertices);

		for (unsigned int j = 0; j < m->mNumFaces; ++j)
		{
			const aiFace& tri = m->mFaces[j];

			m_indices.push_back(tri.mIndices[0]);
			m_indices.push_back(tri.mIndices[1]);
			m_indices.push_back(tri.mIndices[2]);
		}

		num_vertices += m->mNumVertices;
	}
}

void SkeletonMesh::InitBone(const aiMesh* mesh, int index)
{
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		const aiBone* b = mesh->mBones[i];
		int bone_id = m_skeleton->getBoneID(b->mName.C_Str());
		//std::cout << "bone name: " << b->mName.C_Str() << " bone id: " << bone_id << std::endl;

		for (unsigned int j = 0; j < b->mNumWeights; ++j)
		{
			//std::cout << "bone_id: " << bone_id << " vertex_id: " << b->mWeights[j].mVertexId << " weight: " << b->mWeights[j].mWeight << std::endl;
			const aiVertexWeight& wt = b->mWeights[j];
			int vertexId = index + b->mWeights[j].mVertexId;
			m_bones[vertexId].AddBoneData(bone_id, wt.mWeight);
		}
	}
}

int SkeletonMesh::GetBoneId(const aiBone* b)
{
	std::string name(b->mName.C_Str());

	auto it = m_name_index.find(name);
	if(it == m_name_index.end())
		m_name_index[name] = (int)m_name_index.size();

	return m_name_index[name];
}
