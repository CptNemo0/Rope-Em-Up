#ifndef HITBOX_RENDERER_H
#define HITBOX_RENDERER_H

#include "../GameObject.h"
#include "Component.h"
#include "glm/glm.hpp"
#include "../Texture.h"
#include <memory>
#include "../res/Resources.h"
#include "../Random.h"
namespace components
{
	class HitboxRenderer : public Component, public std::enable_shared_from_this<HitboxRenderer>
	{
	public:
		float vertices_data_[20]
		{
			 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //lb
			 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //rb
			 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //rt
			 0.0f, 0.0f, 0.0f, 0.0f, 0.0f  //lt
		};

		unsigned int indices_data_[6]{ 1, 3, 2, 0, 2, 1 };

		unsigned int vao_;
		unsigned int vbo_;
		unsigned int ebo_;

		std::shared_ptr<tmp::Texture> texture_;

		glm::vec4 color_;

		float percentage_;

		HitboxRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt);
		void Draw();

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}

#endif // !HITBOX_RENDERER_H