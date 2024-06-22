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

    float x = 0, y = 0;

    for (auto c : text_)
    {
        if (c == '\n')
        {
            x = 0;
            y -= line_spacing_;
            continue;
        }

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

components::TextRenderer::TextRenderer(json &j)
{
    auto shader = res::get_shader(j["shader_paths"][0], j["shader_paths"][1]);
    auto font = res::get_font(j["font_path"]);
    auto text = j["text"];
    auto color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);

    this->TextRenderer::TextRenderer(shader, font, text, color);
}

json components::TextRenderer::Serialize()
{
    json j;

    j["shader_paths"] = {shader_->v_path, shader_->f_path};
    j["font_path"] = font_->path_;
    j["text"] = text_;
    j["color"] = {color_.r, color_.g, color_.b};

    return j;
}
