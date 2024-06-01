#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "animation/AssimpGLMHelpers.h"
#include "animation/AnimData.h"
#include "glad/glad.h"

#include "global.h"
#include "Vertex.h"
#include "Mesh.h"
#include <map>

unsigned int TextureFromFile(const char* path, const string& directory);


class Model
{
public:
		std::vector<s_ptr<Mesh>> meshes_;
		std::vector <Texture> textures_loaded_; //na razie nie robi�am oddzielnych materia��w bo tak by�o w sumie szybciej i nie wiem czy klasa Model jest nam rzeczywi�cie potrzebna D:
		string directory_;
		bool gammaCorrection;
		string path_;

		Model(string path, bool gamma = false);

		void Draw(s_ptr<Shader> shader) const;
		void processNode(aiNode* node, const aiScene* scene);
		s_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector <Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


		/*skeleton*/
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		int m_BoneCounter = 0;

		auto& GetBoneInfoMap() { return m_BoneInfoMap; }
		int& GetBoneCount() { return m_BoneCounter; }

		void SetVertexBoneDataToDefault(Vertex& vertex);
		void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
		
};


#endif //MODEL_H

