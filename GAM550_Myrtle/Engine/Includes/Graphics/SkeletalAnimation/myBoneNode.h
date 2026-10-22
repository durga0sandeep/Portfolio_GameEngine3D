/*!****************************************************************************
 * file  : myBoneNode.h
 * brief : Class from the Node to build the tree of AiNode
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>              // Core types like vec3, mat4
#include <glm/gtc/quaternion.hpp>   // For glm::quat
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, rotate, etc.
//#include <glm/gtx/transform.hpp>    // Optional: extra transform utilities

/*!****************************************************************************
 * not really use in this class but toMatrix is use when convert VQS
 * normally Scale should be a float but the model only support vec3 scale
 *****************************************************************************/
struct VQS {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 toMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 R = glm::mat4_cast(rotation);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
        return T * R * S;
    }
};

class myBoneNode
{
public:
	//Bone(Bone* parent,const std::string& name, glm::mat4 globalTransform, std::vector<int> children);
    myBoneNode(myBoneNode* parent, const std::string& name)
        :m_parent(parent), m_name(name){};
	void setParent(myBoneNode* parent);
	void addChild(myBoneNode* child);
    void updateGlobalTransform(); // Recursively update global transforms
    const glm::mat4& getGlobalTransform() const { return m_globalTransform; }
    const glm::mat4& getLocalTransform() const { return m_localTransform; }
    myBoneNode* getParent() const { return m_parent; }
	void setName(const std::string& name) { m_name = name; }
    void setLocalTransform(glm::mat4 matrix); // Update local transform from position, rotation, scale
	const std::string& getName() const { return m_name; }
	myBoneNode* getParent() { return m_parent; }

private:
	
    glm::mat4 m_localTransform;  // Local transformation matrix
    glm::mat4 m_globalTransform; // Global transformation matrix (after applying parent transforms)
    myBoneNode* m_parent = nullptr; // Pointer to parent bone (nullptr if root)
	std::string m_name; // Name of the bone for identification
	std::vector<myBoneNode*> m_children; // IDs of child bones
};

