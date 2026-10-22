#pragma once


class SSBO
{
    private:
        unsigned int ssboID;

    public:
        void setupSSBO() {
            glGenBuffers(1, &ssboID);   // Create the buffer object
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboID); // Bind to binding point
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind
        }
        void updateSSBO(std::vector<pointLightData>& pointLightsVec, int structSize);

};

