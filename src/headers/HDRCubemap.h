#ifndef HDRCUBEMAP_H
#define HDRCUBEMAP_H

#include "glad/glad.h"
#include "string"
#include <iostream>

#include "global.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"	
#include "GLFW/glfw3.h"

class HDRCubemap
{
public:
	string path_;
	s_ptr<Shader> BackgroundShader_;
	s_ptr<Shader> EquirectangularToCubemapShader_;
	s_ptr<Shader> IrrandanceShader_;
	s_ptr<Shader> PrefilterShader_;
	s_ptr<Shader> BrdfShader_;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int envCubemap = 0;
	unsigned int irradanceMap = 0;
	unsigned int prefilterMap = 0;
	unsigned int brdfLUT = 0;

	HDRCubemap(const string& path, s_ptr<Shader> BackgroundShade, 
		s_ptr<Shader> EquirectangularToCubemapShader, s_ptr<Shader> IrrandanceShader, s_ptr<Shader> PrefilterShader, s_ptr<Shader> BRDFShader);
	~HDRCubemap() = default;

	void LoadHDRimg(GLFWwindow* window, s_ptr<llr::Camera> camera);
	void RenderCube();
	void RenderQuad();
	void BindIBLmaps(s_ptr<Shader> shader);
	void BindIrradianceMap(s_ptr<Shader> shader);
	void BindEnvCubemap(s_ptr<Shader> shader);

};

#endif // !HDRCUBEMAP_H