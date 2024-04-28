#include "../../headers/components/TextRenderer.h"

components::TextRenderer::TextRenderer(s_ptr<Shader> shader, s_ptr<Font> font, string text, glm::vec3 color)
{
    shader_ = shader;
    text_ = text;
    color_ = color;
    font_ = font;
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  
}

void components::TextRenderer::Start()
{
    transform_ = gameObject_.lock()->transform_;
}

void components::TextRenderer::Update()
{
    shader_->SetInt("tex", 0);
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
    shader_->SetVec3("tex_color", color_);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        cout << "OpenGL Error: " << error << endl;
    }

    float x = 0, y = 0;

    for (auto c : text_)
    {
        auto &glyph = font_->glyphs_[c];

        float x_pos = x + glyph.bearing_x;
        float y_pos = y - (glyph.height - glyph.bearing_y);

        float vertices[24] = {
            x_pos, (y_pos + glyph.height), 0.0f, 0.0f,
            x_pos, y_pos, 0.0f, 1.0f,
            (x_pos + glyph.width), y_pos, 1.0f, 1.0f,

            x_pos, (y_pos + glyph.height), 0.0f, 0.0f,
            (x_pos + glyph.width), y_pos, 1.0f, 1.0f,
            (x_pos + glyph.width), (y_pos + glyph.height), 1.0f, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, glyph.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        x += glyph.advance >> 6;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
