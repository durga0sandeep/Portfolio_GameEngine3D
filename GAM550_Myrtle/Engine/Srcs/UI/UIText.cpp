#include "pch.h"
#include "UI/UIText.h"
#include <Engine.h>

std::map<char, Character> UIText::glyphs;
FT_Library UIText::ft;
FT_Face UIText::face;
std::shared_ptr<Shader> UIText::textShader;
std::shared_ptr<Mesh> UIText::textMesh;
std::vector<std::shared_ptr<Texture>> UIText::glyphTextures;

/**
 * $BRIEF
 * initialize text resources, include font loading and shader setup
 * \param fontPath
 */
void UIText::initializeTextResources(const std::string& fontPath) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType\n";
        return;
    }
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Failed to load font\n";
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    loadGlyphs();

    UIText::textMesh = std::make_shared<Mesh>("UIText");
    UIText::textShader = std::make_shared<Shader>("Shader/text_vertex_shader.vertex\nShader/text_fragment_shader.fragment");
    //UIText::textShader = std::make_shared<Shader>("Shader/button_vertex_shader.vertex\nShader/button_fragment_shader.fragment");

    std::vector<float> vertices = {
        // x, y,    
        0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f,  // top-right
        0.0f, 1.0f   // top-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };
    textMesh->setVertexData(
        {
            {GeometryBuffer::Attribute::Position, vertices, 2},
        },
        2 * sizeof(float), // stride: 2 pos + 2 texCoord
        indices
        );
}

/**
 * $BRIEF
 * help function to load glyphs from the font face
 */
void UIText::loadGlyphs() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character ch = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        glyphs.insert(std::pair<char, Character>(c, ch));
        glyphTextures.push_back(std::make_shared<Texture>(texture, (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows, 1, GL_RED, Texture::Type::Texture2D));
    }
}

/**
 * $BRIEF
 * constructor for UIText
 * \param pos
 * \param text
 * \param textScale
 * \param textColor
 */
UIText::UIText(glm::vec2 pos, const std::string& text, float textScale, glm::vec3 textColor):
    position(pos), content(text), scale(textScale), color(textColor)
{
    textMaterial = std::make_shared<Material>(UIText::textShader);
}

/**
 * $BRIEF
 * draw the text to the screen
 */
void UIText::draw() const {
    textMaterial->setProperty("position", position);
    textMaterial->setProperty("screenSize", Engine::instance().getWindowSize());
    textMaterial->setProperty("textColor", color);


    glm::vec2 pos = position;

    for (char c : content) {
        
        Character ch = glyphs[c];

        

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        textMaterial->setProperty("position", pos);
        textMaterial->setProperty("size", glm::vec2(w* scale, h*scale));
        textMaterial->setTexture("textTexture", glyphTextures[c],0);

        textMaterial->apply();
        pos += glm::vec2(w * scale, 0); // bitshift by 6 to get value in pixels (1/64th of a pixel)
        //bind vao
        textMesh->getGeometryBuffer()->bind();
        
        //textMesh->getGeometryBuffer()->updateVertexAttribute(GeometryBuffer::Attribute::Position, vertices);
        if (textMesh->getGeometryBuffer()->getIndexCount() > 0) {
            glDrawElements(GL_TRIANGLES, textMesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        
        textMesh->getGeometryBuffer()->unbind();
    }
}

