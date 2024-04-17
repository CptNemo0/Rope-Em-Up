#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glad/glad.h"
#include "Texture.h"

#include "Vertex.h"

class Material
{
public:
    unsigned int albedoMap;
    unsigned int normalMap;
    unsigned int metallicMap;
    unsigned int roughnessMap;
    unsigned int aoMap;
    std::vector<Texture> textures_loaded;

    Material(Texture albedoMap,
        Texture normalMap,
        Texture metallicMap,
        Texture roughnessMap,
        Texture aoMap) {
        textures_loaded.push_back(albedoMap);
        textures_loaded.push_back(normalMap);
        textures_loaded.push_back(metallicMap);
        std::cout << "Material created" << std::endl;

    }
    Material() = default;


    ~Material();

private:

    };

#endif // MATERIAL_H