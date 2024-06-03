#include "../../headers/components/GrassRenderer.h"

#include "../../headers/GrassRendererManager.h"

namespace components
{
	GrassRenderer::GrassRenderer(glm::vec3 ul, glm::vec3 dr, std::shared_ptr<Model> model, float density)
		: model_(model)
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->indices_.size() * sizeof(unsigned int), &mesh_->indices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, grass_vbo_);
		glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(Vertex), &mesh_->vertices_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		GeneratePositions();

		glBindBuffer(GL_ARRAY_BUFFER, instnace_buffer_);
		glBufferData(GL_ARRAY_BUFFER, grass_positions_.size() * sizeof(float), &grass_positions_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);
	}
	GrassRenderer::GrassRenderer(float radius, std::shared_ptr<Model> model, float density)
		: model_(model)
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->indices_.size() * sizeof(unsigned int), &mesh_->indices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, grass_vbo_);
		glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(Vertex), &mesh_->vertices_[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		GeneratePositions();

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
			grass_positions_.reserve(density_);
			for (int i = 0; i < density_; i++)
			{
				float border = random::RandFloat(0.8f, 1.2f);
				grass_positions_.push_back(random::RandFloat((position.x - radius_) * border, (position.x + radius_) * border));
				grass_positions_.push_back(random::RandFloat((position.z - radius_) * border, (position.z + radius_) * border));
			}

			break;
		case components::RECTANGLE:
			grass_positions_.reserve(density_ * density_);
			float x_dim = std::fabs(dr_.x) - std::fabs(ul_.x);
			float y_dim = std::fabs(dr_.z) - std::fabs(ul_.z);

			float x_step = std::fabs(x_dim) / density_;
			float y_step = std::fabs(y_dim )/ density_;

			for (float i = -x_step; i > (-x_step * (density_ - 1)); i -= x_step)
			{
				for (float j = -y_step; j > (-y_step * (density_ - 1)); j -= y_step)
				{
					grass_positions_.push_back(ul_.x + i + random::RandFloat(-x_step * 0.9f, x_step * 0.9f));
					grass_positions_.push_back(ul_.z + j + random::RandFloat(-y_step * 0.9f, y_step * 0.9f));
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
		GrassRendererManager::i_->RemoveRenderer(shared_from_this());
	}

    GrassRenderer::GrassRenderer(json &j)
    {
		GRASS_PATCH_TYPE patch_type = j["patch_type"];
		int density = j["density"];
		string model_path = j["model_path"];
		
		switch (patch_type)
		{
			case GRASS_PATCH_TYPE::CIRCLE:
			{
				this->GrassRenderer::GrassRenderer(j["radius"], res::get_model(model_path), density);
				break;
			}
			case GRASS_PATCH_TYPE::RECTANGLE:
			{
				auto ul = glm::vec3(j["ul"][0], j["ul"][1], j["ul"][2]);
				auto dr = glm::vec3(j["dr"][0], j["dr"][1], j["dr"][2]);
				this->GrassRenderer::GrassRenderer(ul, dr, res::get_model(model_path), density);
				break;
			}
		}
    }

    json GrassRenderer::Serialize()
    {
        json j;
		j["model_path"] = model_->path_;
		j["density"] = density_;
		j["patch_type"] = patch_type_;
		
		switch (patch_type_)
		{
			case GRASS_PATCH_TYPE::CIRCLE:
				j["radius"] = radius_;
				break;
			case GRASS_PATCH_TYPE::RECTANGLE:
				j["ul"] = { ul_.x, ul_.y, ul_.z };
				j["dr"] = { dr_.x, dr_.y, dr_.z };
				break;
		}
		
		return j;
    }
}