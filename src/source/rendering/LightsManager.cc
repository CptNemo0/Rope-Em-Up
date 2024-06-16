#include "./headers/rendering/LightsManager.h"

void LightsManager::RenderFromLightPov(int lightID)
{
	DepthShader_->Use();
	DepthShader_->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void LightsManager::InitCubeShadowMap(int lightID)
{
	glGenFramebuffers(1, &depthMapFBO[lightID]);
	glGenTextures(1, &cubeShadowMap[lightID]);
	glBindTexture(GL_TEXTURE_2D, cubeShadowMap[lightID]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[lightID]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cubeShadowMap[lightID], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightsManager::InitPlaneShadowMap(int lightID)
{
	glGenFramebuffers(1, &depthMapFBO[lightID + 16]);
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

void LightsManager::Initialize()
{
	for (unsigned int i = 0; i < MAX_LIGHTS; i++)
	{
		InitCubeShadowMap(i);
		InitPlaneShadowMap(i);
	}
}

void LightsManager::BindCubeShadowMap(s_ptr<Shader> shader)
{
	for (unsigned int i = 0; i < MAX_LIGHTS; i++)
	{
		glActiveTexture(GL_TEXTURE10 + i);
		glBindTexture(GL_TEXTURE_2D, cubeShadowMap[i]);
		shader->SetInt("cube_shadow_maps[" + std::to_string(i) + "]", 10 + i);
	}
}

void LightsManager::BindPlaneShadowMap(s_ptr<Shader> shader)
{
	for (unsigned int i = 0; i < MAX_LIGHTS; i++)
	{
		glActiveTexture(GL_TEXTURE16 + i);
		glBindTexture(GL_TEXTURE_2D, planeShadowMap[i]);
		shader->SetInt("plane_shadow_maps[" + std::to_string(i) + "]", 16 + i);
	}
}

