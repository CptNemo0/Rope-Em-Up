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
    std::vector<std::shared_ptr<Texture>> textures;

    Material(std::vector<std::shared_ptr<Texture>> albedoMap,
        std::vector<std::shared_ptr<Texture>> normalMap,
        std::vector<std::shared_ptr<Texture>> metallicMap,
        std::vector<std::shared_ptr<Texture>> roughnessMap,
        std::vector<std::shared_ptr<Texture>> aoMap) {
        
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    ~Material();

private:

    };

#endif // MATERIAL_H