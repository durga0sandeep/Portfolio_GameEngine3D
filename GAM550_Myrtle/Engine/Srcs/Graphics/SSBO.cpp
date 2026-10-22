#include "pch.h"
#include "Graphics/LocalLight.h"
#include "Graphics/SSBO.h"



void SSBO::updateSSBO(std::vector<pointLightData>& pointLightsVec, int structSize) {

    // Check if vector is empty
    if (pointLightsVec.empty()) {
        std::cout << "SSBO Error: pointLightsVec is empty.\n";
        return;   // Or decide what to do here
    }

    // Check if data() is valid
    if (pointLightsVec.data() == nullptr) {
        std::cout << "SSBO Error: data() returned null.\n";
        return;
    }


   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
    size_t lightsSize = pointLightsVec.size() * structSize;
    glBufferData(GL_SHADER_STORAGE_BUFFER, lightsSize, pointLightsVec.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind
    //CHECKERROR;
}







