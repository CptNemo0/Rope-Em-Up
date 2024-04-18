#include "../../headers/components/LegacyTextRenderer.h"

components::LegacyTextRenderer::LegacyTextRenderer(std::shared_ptr<Shader> shader, std::string text)
{
    this->shader_ = shader;

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    ChangeText(text);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void components::LegacyTextRenderer::ChangeText(std::string text)
{
    int size = text.length() * 300;
    std::vector<float> temp_vertices_buffer;
    temp_vertices_buffer.resize(size);

    stb_easy_font_spacing(0.5f);

    quads_count_ = stb_easy_font_print(0.0f, 0.0f, (char*)(text.c_str()), nullptr, temp_vertices_buffer.data(), size);

    int new_size = size * (3.0f / 4.0f);
    vertices_buffer_.resize(new_size);
    int k = 0;
    for (int i = 0; i < quads_count_; i++)
    {
        vertices_buffer_[k] = temp_vertices_buffer[i * 16];
        vertices_buffer_[k + 1] = temp_vertices_buffer[i * 16 + 1];

        vertices_buffer_[k + 2] = temp_vertices_buffer[i * 16 + 4];
        vertices_buffer_[k + 3] = temp_vertices_buffer[i * 16 + 5];

        vertices_buffer_[k + 4] = temp_vertices_buffer[i * 16 + 8];
        vertices_buffer_[k + 5] = temp_vertices_buffer[i * 16 + 9];

        vertices_buffer_[k + 6] = temp_vertices_buffer[i * 16 + 8];
        vertices_buffer_[k + 7] = temp_vertices_buffer[i * 16 + 9];

        vertices_buffer_[k + 8] = temp_vertices_buffer[i * 16 + 12];
        vertices_buffer_[k + 9] = temp_vertices_buffer[i * 16 + 13];

        vertices_buffer_[k + 10] = temp_vertices_buffer[i * 16];
        vertices_buffer_[k + 11] = temp_vertices_buffer[i * 16 + 1];

        k += 12;
    }

    glBufferData(GL_ARRAY_BUFFER, new_size, vertices_buffer_.data(), GL_DYNAMIC_DRAW);
}

void components::LegacyTextRenderer::Start()
{
    this->transform_ = gameObject_.lock()->transform_;
}

void components::LegacyTextRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, quads_count_ * 6.0f);
    glBindVertexArray(0);
}
