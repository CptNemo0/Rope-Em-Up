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
    transition_vignette_time_ = 0.25f;
    transition_vignette_current_time_ = 0.0f;
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
}

void ppc::Postprocessor::Destroy()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void ppc::Postprocessor::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    shader_->SetVec2("resolution", glm::vec2(width_, height_));

    float t = transition_vignette_current_time_ / transition_vignette_time_;
    t = glm::sqrt(t);
    shader_->SetFloat("transition_vignette_amount", 0.0f * t + 1.1f * (1.0f - t));
}
