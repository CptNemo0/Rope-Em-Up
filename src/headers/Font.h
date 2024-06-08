#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <unordered_map>

#include <glad/glad.h>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "typedef.h"

struct Glyph
{
    unsigned int texture_id;
    unsigned int width, height;
    int bearing_x, bearing_y;
    long advance;
};

class Font
{
public:
    Font(FT_Library &ft, const char *font_path);
    string path_;
    std::unordered_map<char, Glyph> glyphs_;
};

#endif // !FONT_H