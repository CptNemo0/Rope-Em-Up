#ifndef HDRCUBEMAP_H
#define HDRCUBEMAP_H

#include "stb_image.h"
#include <string>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "Camera.h"

class HDRCubemap
{
public:
	std::string path_;
	std::shared_ptr<Shader> BackgroundShader_;
	std::shared_ptr<Shader> EquirectangularToCubemapShader_;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int envCubemap;

	HDRCubemap(const std::string& path, std::shared_ptr<Shader> BackgroundShade, std::shared_ptr<Shader> EquirectangularToCubemapShader);
	~HDRCubemap() = default;

	void LoadHDRimg(GLFWwindow* window, std::shared_ptr<llr::Camera> camera);
	void RenderCube();

};

#endif // !HDRCUBEMAP_H