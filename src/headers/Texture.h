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
    std::string type_;
    std::string path_;
    Texture(const std::string& path);
    Texture() = default;

};

#endif //TEXTURE_H

