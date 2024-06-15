#include "../../headers/components/FloorRenderer.h"

unsigned int components::FloorRenderer::floor_tile_vao_ = 0;
unsigned int components::FloorRenderer::floor_tile_vbo_ = 0;
unsigned int components::FloorRenderer::floor_tile_ebo_ = 0;

std::shared_ptr<tmp::Texture> components::FloorRenderer::floor_height_texture_ = nullptr;
std::shared_ptr<tmp::Texture> components::FloorRenderer::floor_normal_texture_ = nullptr;
std::shared_ptr<tmp::Texture> components::FloorRenderer::floor_albedo_texture_ = nullptr;
std::shared_ptr<tmp::Texture> components::FloorRenderer::floor_roughness_texture_ = nullptr;

std::shared_ptr<Shader> components::FloorRenderer::shader_ = nullptr;

glm::mat4 components::FloorRenderer::view_matrix_ = glm::mat4(1.0f);

namespace components
{
    
    void FloorRenderer::Init(std::shared_ptr<Shader> s)
	{
        floor_height_texture_ = res::get_texture("res/models/enviroment/floor/floor_height.png");
        floor_normal_texture_ = res::get_texture("res/models/enviroment/floor/floor_normal.png");
        floor_albedo_texture_ = res::get_texture("res/models/enviroment/floor/floor_albedo.png");
        floor_roughness_texture_ = res::get_texture("res/models/enviroment/floor/floor_roughness.png");
        shader_ = s;

        glGenVertexArrays(1, &floor_tile_vao_);
        glGenBuffers(1, &floor_tile_vbo_);
        glGenBuffers(1, &floor_tile_ebo_);

        glBindVertexArray(floor_tile_vao_);

        glBindBuffer(GL_ARRAY_BUFFER, floor_tile_vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floor_tile_vertices_data), &floor_tile_vertices_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_tile_ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_tile_indices_data), &floor_tile_indices_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
	}
    void FloorRenderer::Draw()
    {
        shader_->Use();

        shader_->SetInt("height_map", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_height_texture_->id_);

        shader_->SetInt("albedo_map", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, floor_albedo_texture_->id_);

        shader_->SetInt("normal_map", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, floor_normal_texture_->id_);

        shader_->SetInt("roughness_map", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, floor_roughness_texture_->id_);
        
        glm::vec3 position = gameObject_.lock()->transform_->get_position();

        shader_->SetVec2("position", glm::vec2(position.x, position.z));
        shader_->SetMatrix4("view_matrix", view_matrix_);
        glBindVertexArray(floor_tile_vao_);
        glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void FloorRenderer::Start()
    {
    }
    void FloorRenderer::Update()
    {
        Draw();
    }
    void FloorRenderer::Destroy()
    {
        if (initialized_)
        {
            glDeleteBuffers(1, &floor_tile_ebo_);
            glDeleteBuffers(1, &floor_tile_vbo_);
            glDeleteVertexArrays(1, &floor_tile_vao_);
            initialized_ = false;
        }
    }
}