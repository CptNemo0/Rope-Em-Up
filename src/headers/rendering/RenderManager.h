#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "./headers/components/MeshRenderer.h"

class RenderManager
{
public:
	static RenderManager* i_;

private:
	RenderManager();
	~RenderManager();

public:
	std::deque<std::shared_ptr<components::MeshRenderer>> mesh_renderer_components_;
	s_ptr<components::MeshRenderer> CreateMeshRendererComponent(s_ptr<Model> model, s_ptr<Shader> shader);
	void AddMeshRendererComponent(s_ptr<components::MeshRenderer> meshRenderer);
	void RemoveMeshRendererComponent(s_ptr<components::MeshRenderer> meshRenderer);

	void SetUpMeshRenderer();
	void RenderFromLightPOV(s_ptr<Shader> depthShader);

	static void Initialize()
	{
		if (i_ == nullptr)
		{
			i_ = new RenderManager();
		}
	}

	static void Destroy()
	{
		if (i_ != nullptr)
		{
			delete i_;
			i_ = nullptr;
		}
	}

};

#endif // !RENDERMANAGER_H

