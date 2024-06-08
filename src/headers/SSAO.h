#ifndef SSAO_H
#define SSAO_H

#include "global.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <random>
#include "Shader.h"

enum SSAOPrecision
{
	LOW_SSAO = 16,
	MEDIUM_SSAO = 64,
	HIGH_SSAO = 128
};

class SSAOBuffer
{
public:
	unsigned int fbo_;
	unsigned int ssao_texture_;
	unsigned int noise_texture_;

	SSAOPrecision quality_;

	unsigned int vao_;
	unsigned int vbo_;

	std::vector<glm::vec3> kernel_;
	std::vector<glm::vec3> noise_;

	SSAOBuffer(int height, int width, SSAOPrecision precision);
	void Init(int height, int width);
	void Bind();
	void Unbind();
	void Draw();
	void GenerateKernel(int precision);
	void GenerateNoise(int precision);
	void BindTextures(s_ptr<Shader> shader, unsigned int position, unsigned int normals, unsigned int mask);
	void SetKernel(s_ptr<Shader> shader);
};

class SSAOBlurBuffer
{
public:
	unsigned int fbo_;
	unsigned int intermediate_texture_;
	unsigned int texture_;

	unsigned int vao_;
	unsigned int vbo_;

	SSAOBlurBuffer(int height, int width);
	void Init(int height, int width);
	void Bind();
	void Unbind();
	void Draw();
	void BindTextures(s_ptr<Shader> shader, unsigned int ssao_texture);
};

#endif // !SSAO_H