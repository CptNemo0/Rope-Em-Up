#include "../../headers/components/HUDRenderer.h"

components::HUDRenderer::HUDRenderer(s_ptr<tmp::Texture> texture, s_ptr<Shader> shader)
{
    this->texture_ = texture;
    this->shader_ = shader;

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void components::HUDRenderer::Start()
{
    this->transform_ = gameObject_.lock()->transform_;
}

void components::HUDRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_.lock()->get_model_matrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id_);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
