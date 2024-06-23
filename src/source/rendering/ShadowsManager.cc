#include "./headers/rendering/ShadowsManager.h"

ShadowsManager* ShadowsManager::i_ = nullptr;

ShadowsManager::ShadowsManager(s_ptr<Shader> depthShader)
{
	depth_shader_ = depthShader;
}

void ShadowsManager::Initialize(s_ptr<Shader> depthShader)
{
	if (i_ == nullptr)
	{
		i_ = new ShadowsManager(depthShader);
	}
}

void ShadowsManager::SetLightSpaceMatrix(glm::vec3 lightPos, int lightID)
{

	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix[lightID] = lightProjection * lightView;
}

void ShadowsManager::SetUpCubeShadowBuffers(int lightID)
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	depth_shader_->Use();
	depth_shader_->SetVec3("lightPos", pointLights[lightID].position);
	depth_shader_->SetFloat("far_plane", farPlane);

	std::vector<glm::mat4> shadowTransforms = GetShadowTransforms(pointLights[lightID].position, farPlane);
	for (unsigned int j = 0; j < 6; ++j)
	{
		depth_shader_->SetMatrix4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowsManager::InitCubeShadowMap(int lightID)
{
	glGenFramebuffers(1, &depthMapFBO[lightID]);
	glGenTextures(1, &cubeShadowMap[lightID]);
	glBindTexture(GL_TEXTURE_2D, cubeShadowMap[lightID]);
	for (unsigned int j = 0; j < 6; ++j)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeShadowMap[lightID], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowsManager::InitPlaneShadowMap(int lightID)
{
	glGenFramebuffers(1, &depthMapFBO[lightID]);
	glGenTextures(1, &planeShadowMap);
	glBindTexture(GL_TEXTURE_2D, planeShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, planeShadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<glm::mat4> ShadowsManager::GetShadowTransforms(glm::vec3 lightPos, float farPlane)
{
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, 1.0f, farPlane);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return shadowTransforms;
}


void ShadowsManager::BindCubeShadowMap(s_ptr<Shader> shader, int lightID)
{

		glActiveTexture(GL_TEXTURE10 + lightID);
		glBindTexture(GL_TEXTURE_2D, cubeShadowMap[lightID]);
		shader->SetInt("cube_shadow_maps[" + std::to_string(lightID) + "]", 10 + lightID);
}

void ShadowsManager::BindPlaneShadowMap(s_ptr<Shader> shader, int lightID)
{
		glActiveTexture(GL_TEXTURE10 + lightID);
		glBindTexture(GL_TEXTURE_2D, planeShadowMap);
		shader->SetInt("plane_shadow_maps[" + std::to_string(lightID) + "]", 10 + lightID);
}

