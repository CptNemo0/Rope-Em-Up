#include "../headers/Font.h"

Font::Font(FT_Library &ft, const char *font_path)
{
    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << endl;
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Glyph glyph = {
            texture,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            face->glyph->bitmap_left, face->glyph->bitmap_top,
            face->glyph->advance.x
        };

        glyphs_.insert({c, glyph});
    }
}
