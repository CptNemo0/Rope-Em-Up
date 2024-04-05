#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "string"
#include "glad/glad.h"
#include "stb_image.h"

class Texture
{
public:
    unsigned int id_;
    const std::string& type_;
    Texture(const std::string& path);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

};

#endif //TEXTURE_H

