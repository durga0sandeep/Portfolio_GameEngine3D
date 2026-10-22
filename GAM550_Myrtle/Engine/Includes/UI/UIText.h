/*!****************************************************************************
 * file  : @file
 * brief : UI Text - The UIText class represents a text element in the UI.
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <ft2build.h>
#include <Graphics/Shader.h>
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>
#include FT_FREETYPE_H

struct Character {
    GLuint textureID;   // Glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing; // Offset from baseline
    GLuint advance;     // Horizontal offset to next glyph
};

class UIText {
public:
    glm::vec2 position;
    std::string content;
    float scale;
    glm::vec3 color;

    std::shared_ptr<Material> textMaterial;
    bool active = true;

    static std::map<char, Character> glyphs;
    static std::vector<std::shared_ptr<Texture>> glyphTextures;
    static std::shared_ptr<Shader> textShader;
    static FT_Library ft;
    static FT_Face face;
    static std::shared_ptr<Mesh> textMesh;

    static void initializeTextResources(const std::string& fontPath);
    static void loadGlyphs();

    UIText(glm::vec2 pos, const std::string& text, float textScale, glm::vec3 textColor);

    void setText(const std::string& newText) { content = newText; }

    void draw() const;

    void setActive(bool in_active)
    {
        active = in_active;
    };

    bool isActive()
    {
        return active;
    }
};


