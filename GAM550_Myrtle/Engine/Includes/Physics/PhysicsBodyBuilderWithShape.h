/*!****************************************************************************
 * file  : PhysicsBodyBuilderWithShape.h
 * brief : apply type-state pattern ensure that use have a shape before building
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "Physics\PhysicsBodyBuilder.h"

class PhysicsBodyBuilderWithShape : public PhysicsBodyBuilder
{
    PhysicalRigidBody build(const CollsionShape& shapeType,
        const glm::vec3& pos,
        const glm::quat& rot);
};

