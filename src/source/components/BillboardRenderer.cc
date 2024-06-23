#include "headers/components/BillboardRenderer.h"
#include "headers/BillboardRendererManager.h"
components::BillboardRenderer::BillboardRenderer(std::shared_ptr<Model> model, std::shared_ptr<tmp::Texture> texture)
{
	model_ = model;
	texture_ = texture;
}

void components::BillboardRenderer::Draw()
{
	model_->Draw();
}

void components::BillboardRenderer::Start()
{
}

void components::BillboardRenderer::Update()
{
}

void components::BillboardRenderer::Destroy()
{
	
	BillboardRendererManager::i_->RemoveRenderer(shared_from_this());
}


