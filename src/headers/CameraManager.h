#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include "Camera.h"

namespace llr
{
	class CameraManager
	{
	public:
		s_ptr<components::CameraComponent> debuggingCamera_;
		s_ptr<components::GameplayCameraComponent> topDownCamera_;
		s_ptr<components::GameplayCameraComponent> isometricCamera_;

		CameraManager();
		CameraManager(s_ptr<components::CameraComponent> debuggingCamera, s_ptr<components::GameplayCameraComponent> topDownCamera, s_ptr<components::GameplayCameraComponent> isometricCamera);
		~CameraManager() = default;

		void setDebuggingCamera(s_ptr<components::CameraComponent> camera);
		void setTopDownCamera(s_ptr<components::GameplayCameraComponent> camera);
		void setIsometricCamera(s_ptr<components::GameplayCameraComponent> camera);




	private:

	};
}


#endif // CAMERAMANAGER_H