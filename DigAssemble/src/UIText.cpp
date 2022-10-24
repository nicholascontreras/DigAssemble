#include "UIText.h"

#include <stdexcept>

#include "util/debug.h"

#include <glad/glad.h>

#include "ShaderProgramManager.h"
#include "TextureManager.h"


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
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ShaderProgramManager::compileProgram("text");
}

void UIText::drawText(const std::string& text, const int& x, const int& y, const int& size, const glm::vec3& color) {

    float scale = (float)size / LOADING_SIZE;

    ShaderProgramManager::setActiveProgram("text");
    ShaderProgramManager::setVec3("textColor", color);

    glBindVertexArray(VAO);

    int curX = x;
    for(std::string::const_iterator curCharItr = text.begin(); curCharItr != text.end(); curCharItr++) {
        CharacterInfo curCharInfo = characters.at(*curCharItr);

        float posX = curX + curCharInfo.bearing.x * scale;
        float poxY = y - (curCharInfo.size.y - curCharInfo.bearing.y) * scale;

        float w = curCharInfo.size.x * scale;
        float h = curCharInfo.size.y * scale;
        // update VBO for each character
        //float vertices[6][4] = {
        //    { posX,     poxY + h,   0.0f, 0.0f },
        //    { posX,     poxY,       0.0f, 1.0f },
        //    { posX + w, poxY,       1.0f, 1.0f },

        //    { posX,     poxY + h,   0.0f, 0.0f },
        //    { posX + w, poxY,       1.0f, 1.0f },
        //    { posX + w, poxY + h,   1.0f, 0.0f }
        //};
        float vertices[6][4] = {
            { 0,     0,   0.0f, 0.0f },
            { 0,     20,       0.0f, 1.0f },
            { 20, 20,       1.0f, 1.0f },

            { 0,     0,   0.0f, 0.0f },
            { 20, 20,       1.0f, 1.0f },
            { 20, 0,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        TextureManager::bindTexture(TEXTURE_PREFIX + *curCharItr);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        curX += (int) ((curCharInfo.advance >> 6) * scale); // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
}

bool UIText::loadChar(const FT_Face& fontFace, const char& c) {
    // load character glyph 
    if(FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
        Debug("ERROR::FREETYTPE: Failed to load Glyph");
        return false;
    }

    std::string textureName = TEXTURE_PREFIX;
    TextureManager::generateTexture(TEXTURE_PREFIX + c, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, fontFace->glyph->bitmap.buffer);

    CharacterInfo character = {
        glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
        glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
        (unsigned int) fontFace->glyph->advance.x
    };
    characters.emplace(c, character);

    return true;
}


