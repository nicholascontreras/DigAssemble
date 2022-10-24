#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H  

class UIText {
public:
    static void init();
    static void drawText(const std::string& text, const int& x, const int& y, const int& size, const glm::vec3& color);

private:
    struct CharacterInfo {
        glm::ivec2   size;       // Size of glyph
        glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
        unsigned int advance;    // Offset to advance to next glyph
    };

    static const std::string TEXTURE_PREFIX;
    static const unsigned int LOADING_SIZE;

    static unsigned int VAO, VBO;
    static std::unordered_map<char, CharacterInfo> characters;
    
    static bool loadChar(const FT_Face& fontFace, const char& c);
};

