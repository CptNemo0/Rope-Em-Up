#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glad/glad.h"

#include "typedef.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"

class Mesh
{
public:
	unsigned int vao_;
	unsigned int ssbo_;

	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	std::vector<Texture> textures_;
	bool isRigged_;

	Mesh() = default;
	//Mesh(string mesh_path);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool isRigged = false);
	~Mesh();

	void Init();
	void Draw(s_ptr<Shader> shader) const;
	void DrawDepth(s_ptr<Shader> shader) const;
	void UpdateTransforms(std::vector<glm::mat4> transforms);
};

#endif //MESH_H


