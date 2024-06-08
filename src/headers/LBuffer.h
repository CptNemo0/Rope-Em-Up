#ifndef L_BUFFER_H
#define L_BUFFER_H

#include "glad/glad.h"
#include "typedef.h"
#include "Shader.h"
#include "glm/glm.hpp"
class LBuffer
{
public:

	unsigned int l_buffer_;
	unsigned int color_texture_;
	unsigned int bloom_texture_;
	unsigned int rbo_;

	unsigned int vao_;
	unsigned int vbo_;

	bool bloom_;
	glm::vec3 bloom_color_;
	float bloom_threshold_;

	LBuffer(int height, int width);
	~LBuffer() = default;

	void Initialize(int height, int width);
	void Bind();
	void Unbind();
	void BindTextures(s_ptr<Shader> shader);
	void Draw();
};

#endif // !L_BUFFER_H
