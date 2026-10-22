#include "pch.h"
#include "Graphics/ShaderProgram.h"

// Reads a specified file into a string and returns the string.  The
// file is examined first to determine the needed string size.
/*
char* ReadFile(const char* name)
{
    std::ifstream f;
    f.open(name, std::ios_base::binary); // Open
    if (f.fail()) { printf("\nERROR! Can't read/find shader file: %s\n\n", name); exit(-1); }
    f.seekg(0, std::ios_base::end);      // Position at end
    int length = f.tellg();              //   to get the length

    char* content = new char[length + 1]; // Create buffer of needed length
    f.seekg(0, std::ios_base::beg);     // Position at beginning
    f.read(content, length);            //   to read complete file
    f.close();                           // Close

    content[length] = char(0);           // Finish with a NULL
    return content;
}
*/

// Modern C++ version of ReadFile (like AssetLoadManager::readShaderFile)
std::string ShaderProgram::readShaderFile(const std::string& filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}




// Creates an empty shader program.
ShaderProgram::ShaderProgram()
{
    programId = glCreateProgram();
}


// Use a shader program
void ShaderProgram::UseShader()
{
    glUseProgram(programId);
}

// Done using a shader program
void ShaderProgram::UnuseShader()
{
    glUseProgram(0);
}

// Read, send to OpenGL, and compile a single file into a shader
// program.  In case of an error, retrieve and print the error log
// string.
/*
void ShaderProgram::AddShader(const char* fileName, GLenum type)
{
    // Read the source from the named file
    char* src = ReadFile(fileName);
    const char* psrc[1] = { src };

    // Create a shader and attach, hand it the source, and compile it.
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, psrc, NULL);
    glCompileShader(shader);
    glAttachShader(programId, shader);
    delete src;

    // Get the compilation status
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    // If compilation status is not OK, get and print the log message.
    if (status != 1) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetShaderInfoLog(shader, length, NULL, buffer);
        printf("Compile log for %s:\n%s\n", fileName, buffer);
        delete buffer;
    }
}
*/

void ShaderProgram::AddShader(const std::string& fileName, GLenum type)
{
    // Read shader source using modern C++ file handling
    std::string shaderCode = readShaderFile(fileName);
    if (shaderCode.empty()) {
        std::cerr << "Failed to read shader: " << fileName << std::endl;
        return;
    }

    const char* psrc = shaderCode.c_str();

    // Create shader, attach source, and compile
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &psrc, nullptr);
    glCompileShader(shader);
    glAttachShader(programId, shader);

    // Check compilation status
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> buffer(length);
        glGetShaderInfoLog(shader, length, nullptr, buffer.data());
        std::cerr << "Shader compile error (" << fileName << "):\n"
            << buffer.data() << std::endl;
    }
}



// Link a shader program after all the shader files have been added
// with the AddShader method.  In case of an error, retrieve and print
// the error log string.
/*
void ShaderProgram::LinkProgram()
{
    // Link program and check the status
    glLinkProgram(programId);
    int status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);

    // If link failed, get and print log
    if (status != 1) {
        int length;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetProgramInfoLog(programId, length, NULL, buffer);
        printf("Link log:\n%s\n", buffer);
        delete buffer;
    }
}
*/

void ShaderProgram::LinkProgram()
{
    // Link the program
    glLinkProgram(programId);

    // Check link status
    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> buffer(length);
        glGetProgramInfoLog(programId, length, nullptr, buffer.data());

        std::cerr << "Shader program link error:\n" << buffer.data() << std::endl;
    }
    else {
        std::cout << "Shader program linked successfully.\n";
    }
}




//For Compute Shader
void ShaderProgram::UseShader_CS(int i) {

    glUseProgram(programId);

}



// the error log string.
void ShaderProgram::LinkProgram_Compute(int PrgId)
{
    // Link program and check the status
    glLinkProgram(PrgId);
    int status;
    glGetProgramiv(PrgId, GL_LINK_STATUS, &status);

    // If link failed, get and print log
    if (status != 1) {
        int length;
        glGetProgramiv(PrgId, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetProgramInfoLog(PrgId, length, NULL, buffer);
        printf("Link log:\n%s\n", buffer);
        delete[] buffer;
    }
}



void ShaderProgram::BindImageTexture(int unit, unsigned int Textureid, const GLenum access, int ShaderId,const char* name) {
    glBindImageTexture(unit, Textureid, 0, GL_FALSE, 0, access, GL_RGBA32F);
    int loc = glGetUniformLocation(ShaderId, name);
    glUniform1i(loc, unit);
}



void ShaderProgram::BindTexture(const int unit, const int programId, const std::string& name)
{
    //
   
}


void ShaderProgram::DispatchComputerShader(int Width, int Height, int group_x, int group_y) {

    glDispatchCompute(Width / group_x, Height / group_y, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}
