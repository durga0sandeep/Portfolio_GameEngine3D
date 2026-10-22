#include "pch.h"
#include "Graphics/SkeletalAnimation/mySkeleton.h"
#include <queue>
#include <iostream>
#include "Graphics/SkeletalAnimation/AssimpGLMHelpers.h"

glm::mat4 mySkeleton::aiMatrix4x4ToGlm(const aiMatrix4x4& aiMat)
{
	glm::mat4 glmMatrix;
	glmMatrix[0][0] = aiMat.a1; glmMatrix[0][1] = aiMat.b1; glmMatrix[0][2] = aiMat.c1; glmMatrix[0][3] = aiMat.d1;
	glmMatrix[1][0] = aiMat.a2; glmMatrix[1][1] = aiMat.b2; glmMatrix[1][2] = aiMat.c2; glmMatrix[1][3] = aiMat.d2;
	glmMatrix[2][0] = aiMat.a3; glmMatrix[2][1] = aiMat.b3; glmMatrix[2][2] = aiMat.c3; glmMatrix[2][3] = aiMat.d3;
	glmMatrix[3][0] = aiMat.a4; glmMatrix[3][1] = aiMat.b4; glmMatrix[3][2] = aiMat.c4; glmMatrix[3][3] = aiMat.d4;

	return glmMatrix;
}

/**
 * $BRIEF
 * all the node is load with BFS, so we can update with a for loop
 * BFS make sure that parent node is always update already
 */
void mySkeleton::update()
{
	// Update global transforms for all bones
	for (int i = 0; i < allNodes.size();i++) {
		allNodes[i]->updateGlobalTransform();
	}
	for(int i=0; i<m_boneInfo.size();i++)
	{
		auto bone = nodeMap[m_boneInfo[i].name];
		m_boneInfo[i].finalTransformation = globalInverseTransform * bone->getGlobalTransform() * m_boneInfo[i].offset;
	}
}

/**
 * $BRIEF
 * discarded during development, cannot find the code
 */
void mySkeleton::applyBindPose()
{
	
}

VQS convertToVQS(const aiMatrix4x4& mat) {
	aiVector3D scaling, position;
	aiQuaternion rotation;
	mat.Decompose(scaling, rotation, position);

	VQS result;
	result.position = glm::vec3(position.x, position.y, position.z);
	result.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	result.scale = glm::vec3(scaling.x, scaling.y, scaling.z);
	return result;
}

glm::mat4 ConvertMatrixToGLM(const aiMatrix4x4& from) {
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

int mySkeleton::getBoneID(const std::string& name) const {
	auto it = name_index.find(name);
	return (it != name_index.end()) ? it->second : -1;
}

/**
 * $BRIEF
 * use to render the spear, still can figure out where is the spear animtion store in the Assimp file struct
 * \param node
 * \param scene
 * \param staticMeshNodes
 */
void CollectStaticMeshNodeNames(aiNode* node, const aiScene* scene, std::vector<std::string>& staticMeshNodes) {
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];

		if (mesh->mNumBones == 0) {
			staticMeshNodes.push_back(node->mName.C_Str());
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		CollectStaticMeshNodeNames(node->mChildren[i], scene, staticMeshNodes);
	}
}

/**
 * $BRIEF
 * init skeleton structure with a file path
 * using BFS to store to All node for easy update
 * \param path
 */
void mySkeleton::init(const std::string& path)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* input = importer.ReadFile(path,
		aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices);

	if (!input || !input->mRootNode) {
		throw std::runtime_error(importer.GetErrorString());
	}

	// Now process all meshes to get bone offset matrices and IDs
	for (unsigned int meshIndex = 0; meshIndex < input->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = input->mMeshes[meshIndex];
		for (unsigned int i = 0; i < mesh->mNumBones; ++i)
		{
			const aiBone* b = mesh->mBones[i];
			std::string name(b->mName.C_Str());
			auto it = name_index.find(name);
			if (it == name_index.end())
				name_index[name] = (int)name_index.size();
			int bone_id = name_index[name];
			if (bone_id == m_boneInfo.size()) {
				BoneInfo bi(AssimpGLMHelpers::ConvertMatrixToGLMFormat(b->mOffsetMatrix));
				bi.name = name;
				m_boneInfo.push_back(bi);
			}
		}
	}

	std::vector<std::string> staticMeshNodes;
	CollectStaticMeshNodeNames(input->mRootNode, input, staticMeshNodes);

	// Print them out
	for (const auto& name : staticMeshNodes) {
		std::cout << "Static mesh node: " << name << std::endl;
	}


	std::queue<aiNode*> boneQueue;
	// Store the root node of the skeleton for future use
	auto root = input->mRootNode;
	allNodes.push_back(std::make_unique<myBoneNode>(nullptr, root->mName.C_Str()));
	allNodes[0]->setLocalTransform(AssimpGLMHelpers::ConvertMatrixToGLMFormat(root->mTransformation));
	globalInverseTransform = glm::inverse(AssimpGLMHelpers::ConvertMatrixToGLMFormat(root->mTransformation));

	nodeMap.insert({ input->mRootNode->mName.C_Str(), allNodes[0].get() });
	boneQueue.push(root);

	
	while (!boneQueue.empty())
	{
		// BFS through the skeleton hierarchy and store each bone in the allBones vector
		auto currentBone = boneQueue.front();
		boneQueue.pop();

		myBoneNode* Current = nodeMap[currentBone->mName.C_Str()];

		// Add children to the queue

		for (unsigned int i = 0; i < currentBone->mNumChildren; i++) {
			auto child = currentBone->mChildren[i];

			allNodes.push_back(std::make_unique<myBoneNode>(Current, child->mName.C_Str()));
			myBoneNode* newBone = allNodes.back().get();

			newBone->setLocalTransform(AssimpGLMHelpers::ConvertMatrixToGLMFormat(child->mTransformation));
			nodeMap.insert({ child->mName.C_Str(), newBone });
			boneQueue.push(child);

			Current->addChild(newBone);
		}
	}

	
}

