#include "pch.h"
#include "Graphics/SkeletalAnimation/myBoneNode.h"
#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp> // Needed for toMat4

void myBoneNode::setParent(myBoneNode* parent)
{
	m_parent = parent;
}

void myBoneNode::addChild(myBoneNode* child)
{
	m_children.push_back(child);
	child->setParent(this);
}

void myBoneNode::updateGlobalTransform()
{
	if (m_parent)
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	else
		m_globalTransform = m_localTransform;
}

void myBoneNode::setLocalTransform(glm::mat4 matrix)
{
	m_localTransform = matrix;
}
