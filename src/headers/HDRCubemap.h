#ifndef HDRCUBEMAP_H
#define HDRCUBEMAP_H

#include "glad/glad.h"
#include "string"
#include <iostream>
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"	
#include "GLFW/glfw3.h"

class HDRCubemap
{
public:
	std::string path_;
	std::shared_ptr<Shader> BackgroundShader_;
	std::shared_ptr<Shader> EquirectangularToCubemapShader_;
	std::shared_ptr<Shader> IrrandanceShader_;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int envCubemap = 0;
	unsigned int irradianceMap = 0;

	HDRCubemap(const std::string& path, std::shared_ptr<Shader> BackgroundShade, 
		std::shared_ptr<Shader> EquirectangularToCubemapShader, std::shared_ptr<Shader> IrrandanceShader);
	~HDRCubemap() = default;

	void LoadHDRimg(GLFWwindow* window, std::shared_ptr<llr::Camera> camera);
	void RenderCube();

};

#endif // !HDRCUBEMAP_H