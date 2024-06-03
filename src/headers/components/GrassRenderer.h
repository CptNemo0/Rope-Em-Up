#ifndef GRASS_RENDERER_H
#define GRASS_RENDERER_H

#include <memory>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include "nlohmann/json.hpp"

#include "Component.h"
#include "../Mesh.h"
#include "../Model.h"
#include "../Shader.h"
#include "../Random.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../res/Resources.h"
#include "../global.h"

namespace components
{
	enum GRASS_PATCH_TYPE
	{
		CIRCLE,
		RECTANGLE
	};

	class GrassRenderer : public Component, public std::enable_shared_from_this<GrassRenderer>
	{
	public:
		unsigned int grass_vao_;
		unsigned int grass_vbo_;
		unsigned int grass_ebo_;
		unsigned int instnace_buffer_;
		float density_;

		s_ptr<Model> model_;
		std::shared_ptr<Mesh> mesh_;

		std::vector<float> grass_positions_;

		GRASS_PATCH_TYPE patch_type_;
	
		glm::vec3 ul_;
		glm::vec3 dr_;
		float radius_;

		GrassRenderer(glm::vec3 ul, glm::vec3 dr, std::shared_ptr<Model> model, float density);
		GrassRenderer(float radius, std::shared_ptr<Model> model, float density);
		void GeneratePositions();

		void Draw();


		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		GrassRenderer(json &j);
		json Serialize() override;
	};
}

	


#endif // !GRASS_RENDERER_H
