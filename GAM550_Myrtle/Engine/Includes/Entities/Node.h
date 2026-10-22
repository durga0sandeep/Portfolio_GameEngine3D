/*!****************************************************************************
 * file  : Node.h
 * brief : Node - Base Node class , Tree Structure
 * Node can be anything in the game engine
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <sol/sol.hpp>
#include "Components/Transform.h"

 /**
  * @class Node
  * @brief Represents a hierarchical element in a scene graph with transform and child management.
  *
  * Node acts as a basic unit in a tree structure, supporting parent-child relationships,
  * local and world transforms, and update/render propagation. It can be extended by
  * other classes (like GameObject) to create a complete scene hierarchy.
  *
  * @note Inherits from std::enable_shared_from_this to allow safe shared pointer handling.
  */
class Node
{
protected:
    // aliases for code clarity
    typedef std::vector<Node*> ChildrenContainer;
    typedef Node* SharedNode;

    size_t        siblingNumber;
    Node*               parent;
    ChildrenContainer   children;
    std::string         name;

    // Transform related properties of a node
    Transform localTransform;
    Transform worldTransform;
    glm::mat4 localMatrix;
    bool isLocalSpace;

    void        updateSiblingNumbers(size_t startIndex);
    static void printNodeRecursive(std::ostream& os, Node* node, int depth);

    friend std::ostream& operator<<(std::ostream& os, Node& node);

    //add transform component automatically

public:

    Node(std::string name);
    virtual ~Node() { std::cout << "Node " << name << " destroyed\n"; }//= default;

    virtual void    removeNode(SharedNode node);
    virtual void    reparent(SharedNode node);
    virtual void    update();

    void            addChild(SharedNode node);
    SharedNode      findNode(SharedNode node);
    SharedNode      findNodeFast(SharedNode node);

    // Getter methods
    Node*                    getParent()    const { return parent; }
    const ChildrenContainer& getChildren()  const { return children; }
    std::string              getName()      const { return name; };

    // Node space data
    void setLocalPosition(const glm::vec3& position);
    void setLocalRotation(const glm::vec3& rotation);
    void setLocalRotation(const glm::quat& rotation);
    void setLocalScale(const glm::vec3& scale);

    glm::vec3 getLocalPosition() const;
    glm::vec3 getLocalRotation() const;
    glm::vec3 getLocalScale() const;

    // Get the Matrix of whatever this node is
    void worldToLocalSpace();
    void localToWorldSpace();
    glm::mat4 getTransformMatrix();

    Transform& getTransform();
};
