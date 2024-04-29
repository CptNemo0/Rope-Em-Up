#include "../headers/RenderManager.h"

RenderManager* RenderManager::i_ = nullptr;

RenderManager::RenderManager(GBuffer* gbuffer, LBuffer* lbuffer, ppc::Postprocessor* postprocessor, s_ptr<Shader> g_buffer_shader, s_ptr<Shader> lighting_pass_shader)
{
	lbuffer_ = lbuffer;
	gbuffer_ = gbuffer_;
	postprocessor_ = postprocessor;
	g_buffer_shader_ = g_buffer_shader;
	lighting_pass_shader_ = lighting_pass_shader;
}

void RenderManager::GeometryPass()
{
	// ? glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gbuffer_->Bind();
	for (auto& renderer : renderers_)
	{
		renderer->Render(g_buffer_shader_);
	}
	gbuffer_->Unbind();
}

void RenderManager::LightingPass(glm::vec3 camera_position)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lbuffer_->Bind();
	lighting_pass_shader_->Use();
	gbuffer_->BindTextures(lighting_pass_shader_);
	lighting_pass_shader_->SetVec3("camera_position", camera_position);
	/*
	* LIGHTS!!
	* LIGHTS!!
	* LIGHTS!!
	*/

	glBindVertexArray(lbuffer_->vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	lbuffer_->Unbind();
}

void RenderManager::PreprocessorPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	postprocessor_->shader_->Use();
	postprocessor_->Update();
	lbuffer_->BindTextures(postprocessor_->shader_);
	glBindVertexArray(postprocessor_->vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
