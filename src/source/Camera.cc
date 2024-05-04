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

llr::GameplayCamera::GameplayCamera()
{
}
