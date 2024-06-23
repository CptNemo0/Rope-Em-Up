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

void ShadowsManager::RenderFromLightPov(int lightID)
{
	depth_shader_->Use();
	depth_shader_->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix[lightID]);


	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void ShadowsManager::InitCubeShadowMap(int lightID)
//{
//	glGenFramebuffers(1, &depthMapFBO[lightID]);
//	glGenTextures(1, &cubeShadowMap[lightID]);
//	glBindTexture(GL_TEXTURE_2D, cubeShadowMap[lightID]);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cubeShadowMap[lightID], 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void ShadowsManager::InitPlaneShadowMap(int lightID)
{
	glGenFramebuffers(1, &depthMapFBO[lightID]);
	glGenTextures(1, &planeShadowMap[lightID]);
	glBindTexture(GL_TEXTURE_2D, planeShadowMap[lightID]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, planeShadowMap[lightID], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void ShadowsManager::InitShadowMaps(int pointLights, int otherLights)
//{
//	for (unsigned int i = 0; i < pointLights; i++)
//	{
//		InitCubeShadowMap(i);
//	}
//	for (unsigned int i = 0; i < otherLights; i++)
//	{
//		InitPlaneShadowMap(i);
//	}
//}

void ShadowsManager::DepthToTexture(glm::vec3 lightPos, int lightID, bool isPointlight)
{
	if (!isPointlight)
	{
		lightID += MAX_LIGHTS;
	}

	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix[lightID] = lightProjection * lightView;
}

void ShadowsManager::BindCubeShadowMap(s_ptr<Shader> shader, int lightID)
{

		glActiveTexture(GL_TEXTURE12 + lightID);
		glBindTexture(GL_TEXTURE_2D, cubeShadowMap[lightID]);
		shader->SetInt("cube_shadow_maps[" + std::to_string(lightID) + "]", 12 + lightID);
}

void ShadowsManager::BindPlaneShadowMap(s_ptr<Shader> shader, int lightID)
{
		glActiveTexture(GL_TEXTURE10 + lightID);
		glBindTexture(GL_TEXTURE_2D, planeShadowMap[lightID]);
		shader->SetInt("plane_shadow_maps[" + std::to_string(lightID) + "]", 10 + lightID);
}

