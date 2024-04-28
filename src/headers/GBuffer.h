#ifndef G_BUFFER
#define G_BUFFER

#include "glad/glad.h"
#include <stdlib.h>  
#include <memory>
#include "Shader.h"

class GBuffer
{
public:
	unsigned int g_buffer_;

	unsigned int position_texture_;
	unsigned int normal_texture_;
	unsigned int albedo_texture_;
	unsigned int mra_texture_;

	unsigned int rbo_;

	std::shared_ptr<Shader> light_pass_shader;

	void Initialize(int height, int width);
	void Bind();
	void Unbind();
	void BindTextures();
};

#endif // !G_BUFFER
