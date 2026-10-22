/*!****************************************************************************
 * file  : UIPanel.h
 * brief : Background element for menus
 * Pure visual element, doesn't participate in focus or input
 *
 * author: YenLing Chang
 * Course: CS550
 * date  : 11/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <Graphics/Texture.h>
#include <Graphics/Material.h>
#include <Graphics/Mesh.h>

class UIPanel {
public:
    UIPanel(glm::vec2 pos, glm::vec2 sz, std::shared_ptr<Texture> tex);
    UIPanel(glm::vec2 pos, glm::vec2 sz, const std::string& texturePath);

    static void initializePanelResources();

    void draw() const;

    void setActive(bool in_active) {
        active = in_active;
    };

    bool isActive()
    {
        return active;
    }

private:
    glm::vec2 position;
    glm::vec2 size;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Material> panelMaterial;

    static std::shared_ptr<Shader> panelShader;
    static std::shared_ptr<Mesh> panelMesh;
    bool active = true;
};

