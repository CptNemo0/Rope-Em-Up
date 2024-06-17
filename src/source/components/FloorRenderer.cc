#include "../../headers/components/FloorRenderer.h"
#include "../../headers/FloorRendererManager.h"

unsigned int components::FloorRenderer::floor_tile_vao_ = 0;
unsigned int components::FloorRenderer::floor_tile_vbo_ = 0;
unsigned int components::FloorRenderer::floor_tile_ebo_ = 0;



namespace components
{   
    void FloorRenderer::Init()
	{
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
    void FloorRenderer::Draw(std::shared_ptr<Shader> shader)
    {
        glm::vec3 position = gameObject_.lock()->transform_->get_global_position();

        shader->SetVec2("position", glm::vec2(position.x, position.z));

        glBindVertexArray(floor_tile_vao_);
        glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void FloorRenderer::Start()
    {
    }
    void FloorRenderer::Update()
    {
        
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

        FloorRendererManager::i_->RemoveRenderer(shared_from_this());
    }
}