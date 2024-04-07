#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glad/glad.h"

#include "Vertex.h"
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
		std::vector<std::shared_ptr<Mesh>> meshes_;
		std::vector <Texture> textures_loaded_; //na razie nie robi³am oddzielnych materia³ów bo tak by³o w sumie szybciej i nie wiem czy klasa Model jest nam rzeczywiœcie potrzebna D:
		std::string directory_;
		bool gammaCorrection;

		Model(std::string path, bool gamma = false);

		void Draw(std::shared_ptr<Shader> shader) const;
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector <Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif //MODEL_H

