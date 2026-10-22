/*
#include "ShaderUniformInspector.h"
#include <iostream>

ShaderUniformInspector::ShaderUniformInspector(GLuint programID)
    : program(programID)
{
}

void ShaderUniformInspector::PrintAllUniforms() const
{
    GLint numUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

    std::cout << "Active uniforms in program " << program << ":\n";

    for (GLint i = 0; i < numUniforms; ++i)
    {
        const GLsizei bufSize = 256;
        GLchar name[bufSize];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, bufSize, &length, &size, &type, name);
        GLint location = glGetUniformLocation(program, name);

        std::cout << "  " << i << ": name=" << name
            << ", type=" << GLTypeToString(type)
            << ", size=" << size
            << ", location=" << location << "\n";
    }
}

std::string ShaderUniformInspector::GLTypeToString(GLenum type) const
{
    switch (type)
    {
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_INT: return "int";
    case GL_INT_VEC2: return "ivec2";
    case GL_INT_VEC3: return "ivec3";
    case GL_INT_VEC4: return "ivec4";
    case GL_BOOL: return "bool";
    case GL_BOOL_VEC2: return "bvec2";
    case GL_BOOL_VEC3: return "bvec3";
    case GL_BOOL_VEC4: return "bvec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_1D: return "sampler1D";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    default: return "other";
    }
}
*/
