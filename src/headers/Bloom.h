#ifndef BLOOM_H
#define BLOOM

#include "stdlib.h"
#include "glad/glad.h"

class Bloom
{
public:
	unsigned int fbo_;
	unsigned int downscaled_;
	unsigned int horizontal_;
	unsigned int vertical_;

	Bloom(int width, int height);
	void Bind();
	void Unbind();
};

#endif // !BLOOM_H
