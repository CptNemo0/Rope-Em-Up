#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

#define MAX_BONE_INFLUENCE 4
#define MAX_BONE_WEIGHTS 4

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

#endif // !VERTEX_H
