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

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	unsigned int vao_;
	unsigned int vbo_;
	unsigned int ebo_;
	unsigned int indices_num_;

	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	std::vector<Texture> textures;


	Assimp::Importer importer_;
	const aiScene* scene_;
	const aiMesh* mesh_;

	Mesh() = default;
	Mesh(std::string mesh_path);
	~Mesh();

	void Init();
	void Draw() const;
};

#endif //MESH_H


