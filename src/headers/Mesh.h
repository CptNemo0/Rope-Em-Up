#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glad/glad.h"

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"

class Mesh
{
public:
	unsigned int vao_;

	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	std::vector<Texture> textures_;




	/*Assimp::Importer importer_;
	const aiScene* scene_;
	const aiMesh* mesh_;*/

	Mesh() = default;
	//Mesh(std::string mesh_path);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	void Init();
	void Draw(std::shared_ptr<Shader> shader) const;
};

#endif //MESH_H


