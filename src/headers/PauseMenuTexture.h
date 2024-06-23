#ifndef PAUSE_MENU_TEXTURE_H
#define PAUSE_MENU_TEXTURE_H
#include "glad/glad.h"
#include <stdlib.h>

#include "typedef.h"
#include "Shader.h"
class PauseMenuTexture
{
public:
	unsigned int fbo_;
	unsigned int id_;

	unsigned int vao_;
	unsigned int vbo_;

	PauseMenuTexture(int width, int height);
	~PauseMenuTexture();
	void Bind();
	void Unbind();
	void Draw();
};

#endif // !PAUSE_MENU_TEXTURE_H
