#ifndef FLOOR_RENDERER_H
#define FLOOR_RENDERER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Component.h"
#include "../GameObject.h"
#include "../res/Resources.h"
#include <memory>
#include "../Texture.h"
#include "../Shader.h"

namespace components
{
	const float floor_tile_vertices_data[20]
    {
        -1.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         1.0f, 0.0f, -1.0f, 0.0f, 1.0f,
         1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f,  1.0f, 0.0f, 0.0f
    };

	const unsigned int floor_tile_indices_data[4]{ 0, 1, 3, 2 };

	class FloorRenderer : public Component, public std::enable_shared_from_this<FloorRenderer>
	{
	public:
		bool initialized_;

		static unsigned int floor_tile_vao_;
		static unsigned int floor_tile_vbo_;
		static unsigned int floor_tile_ebo_;

		static std::shared_ptr<tmp::Texture> floor_height_texture_;
		static std::shared_ptr<tmp::Texture> floor_normal_texture_;
		static std::shared_ptr<tmp::Texture> floor_albedo_texture_;
		static std::shared_ptr<tmp::Texture> floor_roughness_texture_;

		static std::shared_ptr<Shader> shader_;

		static glm::mat4 view_matrix_;

		FloorRenderer() = default;
		~FloorRenderer() = default;
		static void Init(std::shared_ptr<Shader> s);
		void Draw();
		
		void Start() override;

		void Update() override;

		void Destroy() override;

	};
}

#endif // ! FLOOR_RENDERER_H

