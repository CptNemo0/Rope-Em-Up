#include "../headers/LBuffer.h"

LBuffer::LBuffer(int height, int width)
{
	Initialize(height, width);
}

void LBuffer::Initialize(int height, int width)
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(kQuadVertices), &kQuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &l_buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, l_buffer_);

	glGenTextures(1, &color_texture_);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		exit(1337420);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, l_buffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LBuffer::BindTextures(s_ptr<Shader> shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	shader->SetInt("color_texture", 0);
}