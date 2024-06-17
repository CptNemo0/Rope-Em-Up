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

		FloorRenderer() = default;
		~FloorRenderer() = default;
		static void Init();
		void Draw(std::shared_ptr<Shader> shader);
		
		void Start() override;

		void Update() override;

		void Destroy() override;

	};
}

#endif // ! FLOOR_RENDERER_H

