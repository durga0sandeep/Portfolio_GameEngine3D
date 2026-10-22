#include "pch.h"
#include "UI/UIPanel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Memory/AssetLoadManager.h"

std::shared_ptr<Shader> UIPanel::panelShader = nullptr;
std::shared_ptr<Mesh> UIPanel::panelMesh = nullptr;

void UIPanel::initializePanelResources() {
    const std::string shader_path = "Shader/button_vertex_shader.vertex\nShader/button_fragment_shader.fragment";
    UIPanel::panelShader = std::make_shared<Shader>(shader_path);
    UIPanel::panelMesh = std::make_shared<Mesh>("UIPanel");

    std::vector<float> vertices = {
        0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f,  // top-right
        0.0f, 1.0f   // top-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    panelMesh->setVertexData(
        { {GeometryBuffer::Attribute::Position, vertices, 2} },
        2 * sizeof(float),
        indices
    );
}

UIPanel::UIPanel(glm::vec2 pos, glm::vec2 sz, std::shared_ptr<Texture> tex)
    : position(pos), size(sz), m_texture(tex) {
    panelMaterial = std::make_shared<Material>(UIPanel::panelShader);
}

UIPanel::UIPanel(glm::vec2 pos, glm::vec2 sz, const std::string& texturePath)
    : position(pos), size(sz) {
    m_texture = AssetLoadManager::instance().assetTexture(texturePath.c_str());
    panelMaterial = std::make_shared<Material>(UIPanel::panelShader);
}

void UIPanel::draw() const {
    panelMaterial->setProperty("position", position);
    panelMaterial->setProperty("size", size);
    panelMaterial->setProperty("screenSize", glm::vec2(800.0f, 600.0f));
    //panelMaterial->setProperty("buttonColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    panelMaterial->setTexture("ButtonTexture", m_texture, 0);
    panelMaterial->apply();

    panelMesh->getGeometryBuffer()->bind();
    if (panelMesh->getGeometryBuffer()->getIndexCount() > 0) {
        glDrawElements(GL_TRIANGLES, panelMesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
    }
    panelMesh->getGeometryBuffer()->unbind();
}
