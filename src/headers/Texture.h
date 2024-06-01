#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "string"
#include "stb_image.h"
#include <iostream>

#include "global.h"

struct Texture {
    unsigned int id_;
    string type_;
    string path_;
};

namespace tmp
{
    class Texture
    {

    public:
        unsigned int id_;
  
        int width_;
        int height_;
        string path_;
        Texture(const string& path);
    };
}


#endif //TEXTURE_H

