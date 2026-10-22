#pragma once
#include "Graphics/Mesh.h"

class SphereClass : public Mesh
{
public:
    SphereClass(std::string name, unsigned int segments = 32, float radius = 1.0f);
};
