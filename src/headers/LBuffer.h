#ifndef L_BUFFER_H
#define L_BUFFER_H

#include "glad/glad.h"
#include "global.h"
#include "Shader.h"

class LBuffer
{
public:

	unsigned int l_buffer_;
	unsigned int color_texture_;

	unsigned int vao_;
	unsigned int vbo_;

	LBuffer(int height, int width);
	~LBuffer() = default;

	void Initialize(int height, int width);
	void Bind();
	void Unbind();
	void BindTextures(s_ptr<Shader> shader);
};

#endif // !L_BUFFER_H
