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

void llr::Camera::ConfigureTopDown()
{
	position_ = glm::vec3(0.0f, 20.0f, 0.0f);
	pitch_ = -90.0f;
	yaw_ = -90.0f;
	front_ = glm::vec3(0.0f, -1.0f, 0.0f);
	up_ = kForward;
	right_ = kRight;
	view_matrix_ = GetViewMatrix();
}

void llr::Camera::ConfigureIsometric()
{
	position_ = glm::vec3(100.0f, 100.0f, 100.0f);
	pitch_ = -45.0f;
	yaw_ = 45.0f;
	front_ = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	right_ = glm::normalize(glm::cross(front_, up_));
	view_matrix_ = GetViewMatrix();
}

void llr::Camera::ConfigureDebug()
{
	position_ = glm::vec3(0.0f, 20.0f, 5.0f);
	pitch_ = 0.0f;
	yaw_ = -90.0f;
	front_ = glm::vec3(0.0f, 0.0f, -1.0f);
	up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	right_ = glm::normalize(glm::cross(front_, up_));
	view_matrix_ = GetViewMatrix();
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
