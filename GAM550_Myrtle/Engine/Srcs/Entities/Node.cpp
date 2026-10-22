/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Entities\Node.h"

/**
* @brief Constructs a new Node object.
*
* @param name The name of the node.
*/
Node::Node(std::string name) : parent(), name(name), siblingNumber(0), isLocalSpace(true) {}

void Node::addChild(Node::SharedNode child)
{
    children.push_back(child);
    child->siblingNumber = children.size() - 1;
    child->parent = this;
}

Node::SharedNode Node::findNode(SharedNode node)
{
    if (this == node) {
        return this;
    }
    for (auto& child : children) {
        auto found = child->findNode(node);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

Node::SharedNode Node::findNodeFast(SharedNode node)
{
    std::stack<Node*> nodeStack;
    nodeStack.push(this);

    while (!nodeStack.empty())
    {
        Node* currentNode = nodeStack.top();
        nodeStack.pop();

        if (currentNode == node)
        {
            return currentNode;
        }

        for (auto it = currentNode->children.rbegin(); it != currentNode->children.rend(); ++it)
        {
            nodeStack.push(*it);
        }
    }

    return nullptr;
}

/**
* @brief Removes a Child node from the Tree.
*
* @param node A shared pointer to the node to be removed.
*/
void Node::removeNode(Node::SharedNode node)
{
    auto foundNode = findNode(node);

    // no node found, nothing to do
    if (!foundNode)
        return;

    // node found
    // 0. node will always have a parent (root should be present in the scene, always!)
    Node* parent = foundNode->getParent();

    // 1. erase the found node from parent's children vector and update sibling numbers
    size_t removedIndex = foundNode->siblingNumber;
    parent->children.erase(parent->children.begin() + removedIndex); // still have a reference shared in foundNode variable
    parent->updateSiblingNumbers(removedIndex);

    // 2. foundNode has children, and now that we reset the children of parent, let's just push back the new children
    for (auto& child : foundNode->getChildren())
        parent->addChild(child);

    // 3. clear foundNode
    foundNode->children.clear();
    //foundNode->parent.reset();
    foundNode->siblingNumber = 0;

    // foundNode will be automatically destroyed here.
}


/**
* @brief reparent a Child node to this Node.
*
* @param node A shared pointer to the node to be removed.
*/
void Node::reparent(SharedNode dstNode)
{
    //auto thisNode = shared_from_this(); // sasafe usage of 'this'

    Node* parent = getParent();
    // 1. erase the found node from parent's children vector and update sibling numbers
    size_t removedIndex = siblingNumber;
    parent->children.erase(parent->children.begin() + removedIndex); // still have a reference shared in foundNode variable
    parent->updateSiblingNumbers(removedIndex);

    // 2. attach this to dstNode
    dstNode->addChild(this);
}
glm::vec3 reciprocal(glm::vec3 v)
{
    const float largeNumber = 1e30f; // A very large number to represent "infinity"
    return glm::vec3(
        v.x != 0.0f ? 1.0f / v.x : 0.0f,
        v.y != 0.0f ? 1.0f / v.y : 0.0f,
        v.z != 0.0f ? 1.0f / v.z : 0.0f
    );
}

void Node::update()
{
    if (isLocalSpace)
    {
        if (parent)
        {
            glm::mat4 parentWorld = parent->getTransformMatrix();
            worldTransform.setPosition(parentWorld * glm::vec4(localTransform.getPosition(), 1.0));
            worldTransform.setRotation(parent->worldTransform.getRotation() + localTransform.getRotation());
            worldTransform.setScale(parent->worldTransform.getScale() * localTransform.getScale());
        }
        else
        {
            worldTransform = localTransform;
        }
    }
    else
    {
        if (parent)
        {
            glm::mat4 parentWorldInv = parent->worldTransform.getInverseLocalMatrix();
            localTransform.setPosition(parentWorldInv * glm::vec4(worldTransform.getPosition(), 1.0));
            localTransform.setRotation(worldTransform.getRotation() - parent->worldTransform.getRotation());
            localTransform.setScale(worldTransform.getScale() * reciprocal((parent->worldTransform.getScale())));
        }
        else
        {
            localTransform = worldTransform;
        }
    }


    // Update all children
    for (auto& child : children)
    {
        child->update();
    }
}


void Node::worldToLocalSpace()
{
    isLocalSpace = true;
}

void Node::localToWorldSpace()
{
    isLocalSpace = false;
}

void Node::setLocalPosition(const glm::vec3& position)
{
    localTransform.setPosition(position);
    isLocalSpace = true;  // Ensure we're in local space after this operation
}

void Node::setLocalRotation(const glm::vec3& rotation)
{
    localTransform.setRotation(rotation);
    isLocalSpace = true;  // Ensure we're in local space after this operation
}

void Node::setLocalRotation(const glm::quat& rotation)
{
    localTransform.setRotation(rotation);
}

void Node::setLocalScale(const glm::vec3& scale)
{
    localTransform.setScale(scale);
    isLocalSpace = true;  // Ensure we're in local space after this operation
}

glm::vec3 Node::getLocalPosition() const
{
    return localTransform.getPosition();
}

glm::vec3 Node::getLocalRotation() const
{
    return localTransform.getRotation();
}

glm::vec3 Node::getLocalScale() const
{
    return localTransform.getScale();
}

glm::mat4 Node::getTransformMatrix()
{
    return isLocalSpace ? localTransform.getLocalMatrix() : worldTransform.getLocalMatrix();
}

Transform& Node::getTransform()
{
    return localTransform;
}

void Node::updateSiblingNumbers(size_t startIndex)
{
    for (auto i = startIndex; i < children.size(); ++i) {
        children[i]->siblingNumber = i;
    }
}

void Node::printNodeRecursive(std::ostream& os, Node* node, int depth)
{
    if (!node) return;

    // Print indentation
    for (int i = 0; i < depth; ++i) {
        os << "  ";
    }

    // Print current node
    os << "+-" << node->getName() << std::endl;

    // Print children
    for (const auto& child : node->getChildren())
    {
        printNodeRecursive(os, child, depth + 1);
    }
}

// Overload the << operator for Node
std::ostream& operator<<(std::ostream& os, Node& node)
{
    Node::printNodeRecursive(os, &node, 0);
    return os;
}
