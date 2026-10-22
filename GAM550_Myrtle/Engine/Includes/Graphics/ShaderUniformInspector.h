#pragma once
#include "pch.h"
#include <string>


class ShaderUniformInspector
{
public:
    ShaderUniformInspector(GLuint programID);

    // Print all active uniforms in the shader program
    void PrintAllUniforms() const;

private:
    GLuint program;

    std::string GLTypeToString(GLenum type) const;
};


