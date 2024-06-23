#ifndef BILLBOARD_RENDERER_H
#define BILLBOARD_RENDERER_H

#include "Component.h"
#include "../Model.h"
#include "../Shader.h"
#include <memory>
#include "../Texture.h"
namespace components
{
	class BillboardRenderer : public Component, public std::enable_shared_from_this<BillboardRenderer>
	{
	public:
		BillboardRenderer(std::shared_ptr<Model> model, std::shared_ptr<tmp::Texture> texture);
		std::shared_ptr<Model> model_;
		std::shared_ptr<tmp::Texture> texture_;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		void Draw();
	};
}

#endif // !BILLBOARD_RENDERER_H
