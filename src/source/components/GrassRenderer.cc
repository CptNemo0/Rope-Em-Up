#include "../../headers/components/GrassRenderer.h"

namespace components
{
	GrassRenderer::GrassRenderer(glm::vec3 ul, glm::vec3 dr, std::shared_ptr<Model> model, float density)
	{
		mesh_ = model->meshes_[0];
		patch_type_ = RECTANGLE;
		density_ = density;
		ul_ = ul;
		dr_ = dr;

		glGenVertexArrays(1, &grass_vao_);
		glGenBuffers(1, &grass_vbo_);
		glGenBuffers(1, &grass_ebo_);
		glGenBuffers(1, &instnace_buffer_);
		glBindVertexArray(grass_vao_);

		glBindBuffer(GL_ARRAY_BUFFER, grass_vbo_);
		glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(Vertex), &mesh_->vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->indices_.size() * sizeof(unsigned int), &mesh_->indices_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		GeneratePositions();

		glGenBuffers(1, &instnace_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, instnace_buffer_);
		glBufferData(GL_ARRAY_BUFFER, grass_positions_.size() * sizeof(float), &grass_positions_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);
	}
	GrassRenderer::GrassRenderer(float radius, std::shared_ptr<Model> model, float density)
	{
		mesh_ = model->meshes_[0];
		patch_type_ = CIRCLE;
		density_ = density;
		radius_ = radius;

		glGenVertexArrays(1, &grass_vao_);
		glGenBuffers(1, &grass_vbo_);
		glGenBuffers(1, &grass_ebo_);
		glGenBuffers(1, &instnace_buffer_);
		glBindVertexArray(grass_vao_);

		glBindBuffer(GL_ARRAY_BUFFER, grass_vbo_);
		glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(Vertex), &mesh_->vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->indices_.size() * sizeof(unsigned int), &mesh_->indices_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		GeneratePositions();

		glGenBuffers(1, &instnace_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, instnace_buffer_);
		glBufferData(GL_ARRAY_BUFFER, grass_positions_.size() * sizeof(float), &grass_positions_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);
	}

	void GrassRenderer::GeneratePositions()
	{
		switch (patch_type_)
		{
		case components::CIRCLE:

			glm::vec3 position = gameObject_.lock()->transform_->get_global_position();

			for (int i = 0; i < density_; i++)
			{
				float border = random::RandFloat(0.8f, 1.2f);
				grass_positions_.push_back(random::RandFloat((position.x - radius_) * border, (position.x + radius_) * border));
				grass_positions_.push_back(random::RandFloat((position.z - radius_) * border, (position.z + radius_) * border));
			}

			break;
		case components::RECTANGLE:

			float x_dim = std::fabs(dr_.x) - std::fabs(ul_.x);
			float y_dim = std::fabs(dr_.z) - std::fabs(ul_.z);

			float x_step = x_dim / density_;
			float y_step = y_dim / density_;

			for (float i = -x_step; i > (-x_step * (density_ - 1)); i -= x_step)
			{
				for (float j = -y_step; j > (-y_step * (density_ - 1)); j -= y_step)
				{
					grass_positions_.push_back(i + random::RandFloat(-x_step * 0.9f, x_step * 0.9f));
					grass_positions_.push_back(j + random::RandFloat(-y_step * 0.9f, y_step * 0.9f));
				}
			}

			break;
		}
	}

	void GrassRenderer::Draw()
	{
		switch (patch_type_)
		{
		case components::CIRCLE:
			glBindVertexArray(grass_vao_);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh_->indices_.size()), GL_UNSIGNED_INT, 0, density_);
			glBindVertexArray(0);
			break;
		case components::RECTANGLE:
			glBindVertexArray(grass_vao_);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh_->indices_.size()), GL_UNSIGNED_INT, 0, density_ * density_);
			glBindVertexArray(0);
			break;
		default:
			break;
		}

		
	}

	void GrassRenderer::Start()
	{
	}
	void GrassRenderer::Update()
	{
	}
	void GrassRenderer::Destroy()
	{
	}
}