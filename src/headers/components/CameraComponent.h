#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "./headers/Camera.h"

namespace components
{
	class CameraComponent : public Component
	{
	private:
		s_ptr<llr::Camera> camera_;
		llr::CameraMode current_mode_;
	public:
		CameraComponent();
		CameraComponent(llr::CameraMode current_mode_);
		~CameraComponent();

		void Start() override;
		void Update() override;
		void Destroy() override;

		void SetCameraMode(llr::CameraMode mode);

	
	};

}

#endif // CAMERACOMPONENT_H


