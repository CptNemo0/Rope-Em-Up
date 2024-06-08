#ifndef G_BUFFER
#define G_BUFFER

#include "glad/glad.h"
#include <stdlib.h>  
#include <memory>

#include "typedef.h"
#include "Shader.h"

class GBuffer
{
public:
	unsigned int g_buffer_;

	unsigned int world_position_texture_;
	unsigned int normal_texture_;
	unsigned int albedo_texture_;
	unsigned int mra_texture_;
	unsigned int view_position_texture_;
	unsigned int view_normal_texture_;
	unsigned int mask_texture_;
	unsigned int emissive_texture_;

	unsigned int rbo_;

	GBuffer(int height, int width);
	void Initialize(int height, int width);
	void Bind();
	void Unbind();
	void BindTextures(s_ptr<Shader> shader);
};

#endif // !G_BUFFER
