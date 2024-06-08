#include "../headers/SSAO.h"

SSAOBuffer::SSAOBuffer(int height, int width, SSAOPrecision precision)
{
    GenerateKernel(precision);
    GenerateNoise(precision);
    Init(height, width);
    quality_ = precision;
}

void SSAOBuffer::Init(int height, int width)
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
    glBindVertexArray(0);

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    
    glGenTextures(1, &ssao_texture_);
    glBindTexture(GL_TEXTURE_2D, ssao_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width * 0.75f, height * 0.75f, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_texture_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        exit(1337420);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SSAOBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOBuffer::Draw()
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    /*glBindTexture(GL_TEXTURE_2D, ssao_texture_);
    glGenerateTextureMipmap(ssao_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
    glBindTexture(GL_TEXTURE_2D, 0);*/
}

void SSAOBuffer::GenerateKernel(int precision)
{
    kernel_ = std::vector<glm::vec3>(precision, glm::vec3(0.0));
    std::uniform_real_distribution<GLfloat> distribution(0.0, 1.0);
    std::default_random_engine generator;
    float inv_precison = 1.0f / precision;
    
    for (int i = 0; i < precision; i++)
    {
        glm::vec3 sample(distribution(generator) * 2.0 - 1.0, distribution(generator) * 2.0 - 1.0, distribution(generator));
        sample = glm::normalize(sample);
        sample *= distribution(generator);

        float scale = (float)i * inv_precison;
        scale *= scale;
        scale = 0.1f + scale * 0.9f;

        kernel_[i] = sample * scale;
    }
}

void SSAOBuffer::GenerateNoise(int precision)
{
    noise_ = std::vector<glm::vec3>(precision, glm::vec3(0.0));
    std::uniform_real_distribution<GLfloat> distribution(0.0, 1.0);
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(distribution(generator) * 2.0 - 1.0, distribution(generator) * 2.0 - 1.0, 0.0f); 
        noise_[i] = noise;
    }

    glGenTextures(1, &noise_texture_);
    glBindTexture(GL_TEXTURE_2D, noise_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise_[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAOBuffer::SetKernel(s_ptr<Shader> shader)
{
    int nums = (int)kernel_.size();
    for (int i = 0; i < nums; i++)
    {
        string name = "kernel[" + std::to_string(i) + "]";
        shader->SetVec3(name, kernel_[i]);
    }
}

void SSAOBuffer::BindTextures(s_ptr<Shader> shader, unsigned int position, unsigned int normals, unsigned int mask)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, position);
    shader->SetInt("position_texture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normals);
    shader->SetInt("normal_texture", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noise_texture_);
    shader->SetInt("noise_texture", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mask);
    shader->SetInt("mask_texture", 3);
}

SSAOBlurBuffer::SSAOBlurBuffer(int height, int width)
{
    Init(height, width);
}

void SSAOBlurBuffer::Init(int height, int width)
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
    glBindVertexArray(0);

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glGenTextures(1, &intermediate_texture_);
    glBindTexture(GL_TEXTURE_2D, intermediate_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width * 0.75f, height * 0.75f, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediate_texture_, 0);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width * 0.75f, height * 0.75f, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture_, 0);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        exit(1337420);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOBlurBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SSAOBlurBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOBlurBuffer::Draw()
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SSAOBlurBuffer::BindTextures(s_ptr<Shader> shader, unsigned int ssao_texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssao_texture);
    shader->SetInt("ssao_texture", 0);
}
