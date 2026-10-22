/*!****************************************************************************
 * file  : mySkeleton.h
 * brief : Skeleton class that store all the bone and keep the structure
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <assimp/Importer.hpp>      // For Assimp::Importer
#include <assimp/scene.h>           // For aiScene, aiNode, aiMesh, aiBone, etc.
#include <assimp/postprocess.h>     // For post-processing flags
#include "myBoneNode.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

struct BoneInfo {
	std::string name;
	glm::mat4 offset; // From aiBone::mOffsetMatrix 
	glm::mat4 finalTransformation;
	BoneInfo() {};

	BoneInfo(const glm::mat4& Offset)
	{
		offset = Offset;
	}
};

class mySkeleton
{
public:
	// map of bone name to bone node for easy access
	// use for fast update of bone transforms
	std::unordered_map<std::string, myBoneNode*> nodeMap;
	std::unordered_map<std::string, int> name_index;
	std::vector<BoneInfo> m_boneInfo;
	std::vector<std::string> staticNodeName;

	//this is the tree, the nameing of the BoneNode is just a bad naming for the AINode
	std::vector<std::unique_ptr<myBoneNode>> allNodes; // Store all Node to manage memory
	/*std::vector<StaticAttachment> staticAttachments;*/
	int getBoneID(const std::string& name) const;
	// Pointer to the root bone of the skeleton
	void init(const std::string& path);
	void update();
	void applyBindPose();
	glm::mat4 globalInverseTransform; // Inverse of the root bone's global transform, set externally after loading the skeleton
private:
	glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
};

