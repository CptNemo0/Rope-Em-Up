#pragma once
#include "stb_image.h"
#include <string>
#include "Shader.h"

class HDRCubemap
{
public:

	HDRCubemap(const std::string& path, std::shared_ptr<Shader> shader);
	~HDRCubemap();


void LoadHDRimg(const std::string& path);
void RenderCube();

};

