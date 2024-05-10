#include "./headers/CameraManager.h"


llr::CameraManager::CameraManager()
{
}

llr::CameraManager::CameraManager(s_ptr<components::CameraComponent> debuggingCamera, s_ptr<components::GameplayCameraComponent> topDownCamera, s_ptr<components::GameplayCameraComponent> isometricCamera)
{
	debuggingCamera_ = debuggingCamera;
	topDownCamera_ = topDownCamera;
	isometricCamera_ = isometricCamera;
}

void llr::CameraManager::setDebuggingCamera(s_ptr<components::CameraComponent> camera)
{
	debuggingCamera_ = camera;
}

void llr::CameraManager::setTopDownCamera(s_ptr<components::GameplayCameraComponent> camera)
{
		topDownCamera_ = camera;
}

void llr::CameraManager::setIsometricCamera(s_ptr<components::GameplayCameraComponent> camera)
{
		isometricCamera_ = camera;
}
