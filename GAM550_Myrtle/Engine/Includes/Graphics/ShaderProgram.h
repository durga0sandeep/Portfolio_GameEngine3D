#pragma once
//#include "pch.h"

class ShaderProgram
{
public:
    int programId;
    GLuint BlurSMTextureId, FinalSMTextureId;


    ShaderProgram();//constructor
    //void AddShader(const char* fileName, const GLenum type);
    void AddShader(const std::string& fileName, GLenum type);
    void LinkProgram();
    void UseShader();
    void UnuseShader();


    //ShaderProgram(int i);//constructor
    void LinkProgram_Compute(int PrgId);
    void UseShader_CS(int i);

    //void CreateTexture(int Width, int Height, unsigned int TextureId);
    void BindImageTexture(int unit, unsigned int Textureid, const GLenum access, int ShaderId, const char* name);
    void BindTexture(const int unit, const int programId, const std::string& name);
    void DispatchComputerShader(int Width, int Height, int group_x, int group_y);
    std::string readShaderFile(const std::string& filePath);
};

