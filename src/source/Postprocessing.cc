#include "../headers/Postprocessing.h"

ppc::Postprocessor::Postprocessor(int height, int width, s_ptr<Shader> shader)
{
	width_ = width;
	height_ = height;
    shader_ = shader;

    gamma_ = 1.8f;
    contrast_ = 1.0f;
    brightness_ = 0.0f;
    noise_amount_ = 0.01f;
    vignete_amount_ = 0.25f;
    vignete_contrast_ = 50.0f;
	Init();
}


void ppc::Postprocessor::Init()
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

    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glGenTextures(1, &texture_color_buffer_);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer_, 0);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Destroy();
        exit(69);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ppc::Postprocessor::Destroy()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteFramebuffers(1, &framebuffer_);
    glDeleteRenderbuffers(1, &rbo_);
}

void ppc::Postprocessor::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ppc::Postprocessor::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ppc::Postprocessor::Draw()
{
    Update();
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void ppc::Postprocessor::Update()
{
    shader_->SetVec3("cbg", glm::vec3(contrast_, brightness_, gamma_));
    shader_->SetFloat("noise_amount", noise_amount_);
    shader_->SetFloat("vignete_amount", vignete_amount_);
    shader_->SetFloat("vignete_contrast", vignete_contrast_);
}
