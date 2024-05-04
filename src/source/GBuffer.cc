#include "../headers/GBuffer.h"

GBuffer::GBuffer(int height, int width)
{
	Initialize(height, width);
}

void GBuffer::Initialize(int height, int width)
{
	glGenFramebuffers(1, &g_buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);
	std::cout << "gbuffer: " << g_buffer_ << std::endl;
	glGenTextures(1, &position_texture_);
	glBindTexture(GL_TEXTURE_2D, position_texture_);
	std::cout << "position texture_: " << position_texture_ << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_texture_, 0);

	glGenTextures(1, &normal_texture_);
	glBindTexture(GL_TEXTURE_2D, normal_texture_);
	std::cout << "normal_texture_: " << normal_texture_ << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_texture_, 0);

	glGenTextures(1, &albedo_texture_);
	glBindTexture(GL_TEXTURE_2D, albedo_texture_);
	std::cout << "albedo_texture_: " << albedo_texture_ << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_texture_, 0);

	glGenTextures(1, &mra_texture_);
	glBindTexture(GL_TEXTURE_2D, mra_texture_);
	std::cout << "mra_texture_: " << mra_texture_ << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mra_texture_, 0);

	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		exit(1337);
	}
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::BindTextures(s_ptr<Shader> shader)
{
	glActiveTexture(GL_TEXTURE0);
	shader->SetInt("position_texture", 0);
	glBindTexture(GL_TEXTURE_2D, position_texture_);

	glActiveTexture(GL_TEXTURE1);
	shader->SetInt("normal_texture", 1);
	glBindTexture(GL_TEXTURE_2D, normal_texture_);

	glActiveTexture(GL_TEXTURE2);
	shader->SetInt("albedo_texture", 2);
	glBindTexture(GL_TEXTURE_2D, albedo_texture_);

	glActiveTexture(GL_TEXTURE3);
	shader->SetInt("mra_texture", 3);
	glBindTexture(GL_TEXTURE_2D, mra_texture_);
}