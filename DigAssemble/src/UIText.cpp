#include "UIText.h"

#include <stdexcept>

#include "util/debug.h"

#include <glad/gl.h>

#include <glm/gtc/matrix_transform.hpp>

#include "managers/ShaderProgramManager.h"
#include "managers/TextureManager.h"
#include "GameWindow.h"


const std::string UIText::TEXTURE_PREFIX = "char_";
const unsigned int UIText::LOADING_SIZE = 48;

unsigned int UIText::VAO, UIText::VBO;
std::unordered_map<char, UIText::CharacterInfo> UIText::characters;

void UIText::init() {
    FT_Library ft;
    if(FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Could not init FreeType Library");
    }

    char windowsDir[MAX_PATH];
    GetWindowsDirectory(windowsDir, MAX_PATH);
    std::string fontFile = std::string(windowsDir) + "/Fonts/micross.ttf";

    FT_Face fontFace;
    if(FT_New_Face(ft, fontFile.c_str(), 0, &fontFace)) {
        throw std::runtime_error("Failed to load font");
    }

    FT_Set_Pixel_Sizes(fontFace, 0, LOADING_SIZE);

    for(char i = 32; i < 127; i++) {
        loadChar(fontFace, i);
    }

    FT_Done_Face(fontFace);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
        ShaderProgramManager::compileProgram("text");
}

void UIText::drawText(const std::string& text, int x,  int y, int size, const glm::vec3& textColor) {

    float scale = (float)size / LOADING_SIZE;

    ShaderProgramManager::setActiveProgram("text");
    ShaderProgramManager::setVec3("textColor", textColor);
    ShaderProgramManager::setMat4("projection", glm::ortho(0.0f, (float)GameWindow::getWidth(), (float)GameWindow::getHeight(), 0.0f, -1.0f, 1.0f));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for(std::string::const_iterator curCharItr = text.begin(); curCharItr != text.end(); curCharItr++) {
        CharacterInfo curCharInfo = characters.at(*curCharItr);

        float quadX = x + (curCharInfo.bearing.x * scale);
        float quadY = y + ((curCharInfo.size.y - curCharInfo.bearing.y) * scale);

        float quadW = curCharInfo.size.x * scale;
        float quadH = curCharInfo.size.y * scale;
        
        float vertices[6][4] = {
            { quadX,         quadY,           0.0f, 1.0f },
            { quadX + quadW, quadY,           1.0f, 1.0f },
            { quadX + quadW, quadY - quadH,   1.0f, 0.0f },

            { quadX,         quadY,           0.0f, 1.0f },
            { quadX + quadW, quadY - quadH,   1.0f, 0.0f },
            { quadX,         quadY - quadH,   0.0f, 0.0f }
        };

        TextureManager::bindTexture(TEXTURE_PREFIX + *curCharItr);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (int) ((curCharInfo.advance >> 6) * scale); // bitshift by 6 to get value in pixels (2^6 = 64)
    }

}

bool UIText::loadChar(const FT_Face& fontFace, const char& c) {
    // load character glyph 
    if(FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
        Debug("ERROR::FREETYTPE: Failed to load Glyph");
        return false;
    }

    std::string textureName = TEXTURE_PREFIX;
    TextureManager::generateTexture(TEXTURE_PREFIX + c, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 1, fontFace->glyph->bitmap.buffer);

    CharacterInfo character = {
        glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
        glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
        (unsigned int) fontFace->glyph->advance.x
    };
    characters.emplace(c, character);

    return true;
}


