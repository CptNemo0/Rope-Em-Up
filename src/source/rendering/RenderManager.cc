#include "./headers/rendering/RenderManager.h"

RenderManager* RenderManager::i_ = nullptr;

RenderManager::RenderManager()
{
	mesh_renderer_components_ = std::deque<std::shared_ptr<components::MeshRenderer>>();
}
RenderManager::~RenderManager()
{
	RenderManager::i_->Destroy();
}

s_ptr<components::MeshRenderer> RenderManager::CreateMeshRendererComponent(s_ptr<Model> model, s_ptr<Shader> shader)
{
	auto mesh = std::make_shared<components::MeshRenderer>(model, shader);
	AddMeshRendererComponent(mesh);
	return mesh;
}

void RenderManager::AddMeshRendererComponent(s_ptr<components::MeshRenderer> meshRenderer)
{
	mesh_renderer_components_.push_back(meshRenderer);
}

void RenderManager::RemoveMeshRendererComponent(s_ptr<components::MeshRenderer> meshRenderer)
{
	auto m = std::find(mesh_renderer_components_.begin(), mesh_renderer_components_.end(), meshRenderer);
	if (m != mesh_renderer_components_.end())
	{
		mesh_renderer_components_.erase(m);
	}
}

void RenderManager::SetUpMeshRenderer()
{
	for (auto& mesh : mesh_renderer_components_)
	{
		if (mesh->active_)
		{
			mesh->Start();
		}
	}
}

void RenderManager::RenderFromLightPOV(s_ptr<Shader> depthShader)
{
	for (auto& mesh : mesh_renderer_components_)
	{
		if (mesh->active_ && !mesh->dirty_)
		{
			mesh->Render(depthShader);
		}
	}
}
