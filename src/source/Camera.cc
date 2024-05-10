#include "../headers/Camera.h"

llr::Camera::Camera()
{
	this->position_ = glm::vec3(0.0f);

	glm::vec3 right_ = kRight;
	glm::vec3 up_ = kUp;
	glm::vec3 front_ = kForward;

	this->pitch_ = 0.0f;
	this->yaw_ = 0.0f;

	this->fov_ = 0.0f;
	this->near_ = 0.0f;
	this->far_ = 0.0f;
	this->aspect_ratio_ = 0.0f;
	UpdateDirectionVectors();
}

void llr::Camera::UpdateDirectionVectors()
{
    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front_);

    right_ = glm::normalize(glm::cross(front_, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    up_ = glm::normalize(glm::cross(right_, front_));
}

void components::CameraComponent::Start()
{
}

void components::CameraComponent::Update()
{
	camera_->set_position(transfrom_->get_position());
	camera_->set_right(transfrom_->get_right());
	camera_->set_up(transfrom_->get_up());
}


void components::GameplayCameraComponent::Start()
{
	transfrom_ = gameObject_.lock()->transform_;

	camera_->set_position(transfrom_->get_position());
	camera_->set_right(transfrom_->get_right());
	camera_->set_up(transfrom_->get_up());

	camera_->UpdateDirectionVectors();
}

void components::GameplayCameraComponent::Update()
{
	
	glm::vec3 midPoint = calculateMidPoint();
	glm::vec3 directionToMidpoint = glm::normalize(midPoint - transfrom_->get_position());

//// CHANGING POSITION BY HEIGHT AND DISTANCE OF CAMERA
	glm::vec3 cameraPosition = midPoint + distance_ * directionToMidpoint;

	transfrom_->set_position(glm::vec3(cameraPosition.x ,height_, cameraPosition.z));
	camera_->set_position(transfrom_->get_position());
	camera_->set_position(transfrom_->get_position());

	camera_->UpdateDirectionVectors();
	gameObject_.lock()->transform_ = this->transfrom_;

//// UNCOMMENT THIS TO CHANGE POSITION BY PITCH AND YAW OF CAMERA
	/*float pitch = glm::degrees(asin(directionToMidpoint.y));
	float yaw = glm::degrees(atan2(directionToMidpoint.x, directionToMidpoint.z));

	pitch += pitchAngle_;
	yaw += yawAngle_;

	directionToMidpoint.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	directionToMidpoint.y = sin(glm::radians(pitch));
	directionToMidpoint.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	glm::vec3 cameraPosition = midPoint + distance_ * directionToMidpoint;

	transfrom_->set_position(cameraPosition);
	camera_->set_position(transfrom_->get_position());

	camera_->UpdateDirectionVectors();*/

}

void components::GameplayCameraComponent::Destroy()
{
}

void components::GameplayCameraComponent::RotateCameraAroundMidPoint(float yawAngle)
{
	glm::vec3 midPoint = calculateMidPoint();

	glm::vec3 directionToMidpoint = glm::normalize(midPoint - camera_->get_position());

	glm::vec3 nUp = glm::normalize(camera_->get_up());
	glm::vec3 nRight = glm::normalize(camera_->get_right());

	glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(yawAngle), nUp);

	glm::vec3 rotatedDirection = glm::vec3(yawRotation * glm::vec4(directionToMidpoint, 0.0f));

	glm::vec3 cameraPosition = midPoint + distance_ * rotatedDirection;

	camera_->set_position(cameraPosition);

	glm::mat4 lookAtMatrix = glm::lookAt(cameraPosition, midPoint, nUp);
	glm::quat lookAtQuat = glm::quat_cast(lookAtMatrix);
	camera_->set_front(glm::rotate(lookAtQuat, llr::kForward));

}



components::GameplayCameraComponent::GameplayCameraComponent(s_ptr<GameObject> target1, s_ptr<GameObject> target2, s_ptr <llr::Camera> camera)
{

	target1_ = target1;
	target2_ = target2; 
	camera_ = camera;
	camera_->UpdateDirectionVectors();
}

glm::vec3 components::GameplayCameraComponent::calculateMidPoint()
{
	return (target1_->transform_->get_position() + target2_->transform_->get_position()) / 2.0f;
}

void components::GameplayCameraComponent::SetTargets(s_ptr<GameObject> target1, s_ptr<GameObject> target2)
{
	target1_ = target1;
	target2_ = target2;
}

