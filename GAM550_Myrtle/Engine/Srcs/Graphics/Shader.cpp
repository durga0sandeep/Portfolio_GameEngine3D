/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/Shader.h"
#include "Memory/AssetLoadManager.h"

Shader::Shader()
{

}

Shader::Shader(const std::string& shaderPaths) {
    
    std::istringstream pathsStream(shaderPaths);
    std::string shaderPath;

    while (std::getline(pathsStream, shaderPath)) {
        std::string shaderCode = AssetLoadManager::instance().readShaderFile(shaderPath);
        GLenum shaderType = getShaderType(shaderPath);

        if (shaderType != GL_INVALID_ENUM) {
            unsigned int shaderID = loadShader(shaderCode, shaderType);
            shader_ids[shaderType] = shaderID;
        }
        else {
            std::cerr << "Unrecognized shader type for file: " << shaderPath << std::endl;
        }
    }
    
    linkShaders();
}

Shader::~Shader() {
    glDeleteProgram(program_id);
}

void Shader::use() const {
    glUseProgram(program_id);
}

void Shader::unuse() const {
    glUseProgram(0);
}

void Shader::setUInt(const std::string& name, unsigned int value) const {
    glUniform1ui(getUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::set2Int(const std::string& name, int v0, int v1) const {
    glUniform2i(getUniformLocation(name), v0, v1);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}


void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(getUniformLocation(name), x, y, z, w);
}


void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}


unsigned int Shader::loadShader(const std::string& shaderCode, GLenum shaderType) {
    unsigned int shaderID = glCreateShader(shaderType);
    const char* code = shaderCode.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }

    return shaderID;
}

void Shader::linkShaders() {
    program_id = glCreateProgram();

    //flagColor = glGetUniformLocation(program_id, "Flag");

    for (const auto& [type, id] : shader_ids) {
        glAttachShader(program_id, id);
    }

    glLinkProgram(program_id);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
    }

    for (const auto& [type, id] : shader_ids) {
        glDeleteShader(id);
    }
}

int Shader::getUniformLocation(const std::string& name) const {
    auto it = uniform_location_cache.find(name);
    if (it != uniform_location_cache.end()) {
        return it->second;
    }

    int location = glGetUniformLocation(program_id, name.c_str());
    uniform_location_cache[name] = location;

    if (location == -1) {
        std::cerr << "Shader --> Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
    }

    return location;
}

GLenum Shader::getShaderType(const std::string& filePath) {
    std::string lowercasePath = toLowercase(filePath);
    if (lowercasePath.find("vertex") != std::string::npos) return GL_VERTEX_SHADER;
    if (lowercasePath.find("fragment") != std::string::npos) return GL_FRAGMENT_SHADER;
    if (lowercasePath.find("geometry") != std::string::npos) return GL_GEOMETRY_SHADER;
    if (lowercasePath.find("compute") != std::string::npos) return GL_COMPUTE_SHADER;
    return GL_INVALID_ENUM;
}



std::string Shader::toLowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}




void  Shader::SetFlagColor(int i){
    glUniform1i(flagColor, i);
}

void Shader::setDepthTestOn()
{
    glEnable(GL_DEPTH_TEST);
}


void Shader::setDepthTestOff()
{
    glDisable(GL_DEPTH_TEST);
}

void Shader::setFbo(std::shared_ptr<FBO> fbo)
{
    gFbo = fbo;
}

std::shared_ptr<FBO> Shader::getFbo()
{
    return gFbo;
}


void Shader::BindImageTexture(int unit, unsigned int Textureid, const GLenum access, int ShaderId,const char* name) {
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Textureid);  
    //glBindImageTexture(id, Textureid, 0, GL_FALSE, 0, access, GL_RGBA32F);
    glBindImageTexture(unit, Textureid, 0, GL_FALSE, 0, access, GL_RGBA32F);
    int loc = glGetUniformLocation(ShaderId, name);
    glUniform1i(loc, unit);
}



void Shader::DispatchComputerShader(int Width, int Height, int group_x, int group_y) {

    //glDispatchCompute(Width / group_x, Height / group_y, 1);
    glDispatchCompute(Width , Height, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}



/*

void Shader::activateFbo(int w, int h)
{
    if (fboNeeded == true) {

        GBufferMap_Width = w;
        GBufferMap_Height = h;
        gFbo = new FBO(GBufferMap_Width, GBufferMap_Height);
        gFbo->CreateFBO_Multi(GBufferMap_Width, GBufferMap_Height);
    }
}

void Shader::createFbo_Multi()
{
    gFbo->CreateFBO_Multi(GBufferMap_Width, GBufferMap_Height);
}



void Shader::bindFbo()
{
    gFbo->BindFBO();
}


void Shader::unbindFbo()
{
    gFbo->UnbindFBO();
}


int Shader::getFboDimension()
{
    return gFbo->getFboDimension();
}

*/
