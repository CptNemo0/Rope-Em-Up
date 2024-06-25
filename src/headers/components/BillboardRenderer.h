#ifndef BILLBOARD_RENDERER_H
#define BILLBOARD_RENDERER_H

#include <memory>

#include "Component.h"
#include "../Model.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../components/Transform.h"

namespace components
{
	class BillboardRenderer : public Component, public std::enable_shared_from_this<BillboardRenderer>
	{
	public:
		BillboardRenderer(s_ptr<tmp::Texture> texture, s_ptr<Shader> shader);
		s_ptr<tmp::Texture> texture_;
		s_ptr<components::Transform> transform_;
		s_ptr<Shader> shader_;
		unsigned int VAO_, VBO_;

		glm::vec3 position_offset_ = {0.0f, 0.0f, 0.0f};

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		json Serialize() override;

		void Draw();
	};
}

#endif // !BILLBOARD_RENDERER_H
