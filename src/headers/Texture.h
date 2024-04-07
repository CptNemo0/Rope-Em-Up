#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "string"
#include "glad/glad.h"
#include "stb_image.h"

class Texture
{
public:
    unsigned int id_, width_, height_;
    Texture(const std::string& path, bool alpha = false);
};

#endif //TEXTURE_H

