#include "./headers/components/CameraComponent.h"
using namespace components;

CameraComponent::CameraComponent()
{
	camera_ = make_shared<llr::Camera>();
	current_mode_ = llr::CameraMode::TopDown;
}

components::CameraComponent::CameraComponent(llr::CameraMode current_mode_)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetCameraMode(llr::CameraMode mode) 
{
	current_mode_ = mode;
	switch (mode)
	{
	case llr::CameraMode::TopDown:
		camera_->ConfigureTopDown();
		break;
	case llr::CameraMode::Isometric:
		camera_->ConfigureIsometric();
		break;
	case llr::CameraMode::Debug:
		camera_->ConfigureDebug();
		break;
	}
}

void CameraComponent::Start()
{
	SetCameraMode(current_mode_);
}

void CameraComponent::Update()
{
	camera_->SetViewMatrix(camera_->GetViewMatrix());
}

void CameraComponent::Destroy()
{

}

