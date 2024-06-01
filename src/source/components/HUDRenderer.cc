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

components::HUDRenderer::HUDRenderer(json &j)
{
    auto texture = res::get_texture(j["texture_path"]);
    auto shader = res::get_shader(j["shader_paths"][0], j["shader_paths"][1]);
    this->HUDRenderer::HUDRenderer(texture, shader);
}

json components::HUDRenderer::Serialize()
{
    json j;

    j["texture_path"] = texture_->path_;
    j["shader_paths"] = {shader_->v_path, shader_->f_path};

    return j;
}
