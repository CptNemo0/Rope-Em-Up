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

//void components::CameraComponent::Start()
//{
//}
//
//void components::CameraComponent::Update()
//{
//	camera_->set_position(transfrom_->get_position());
//	camera_->set_right(transfrom_->get_right());
//	camera_->set_up(transfrom_->get_up());
//}


void components::CameraComponent::Start()
{
	transform_ = gameObject_.lock()->transform_;

	camera_->set_position(transform_->get_position());
	camera_->set_right(transform_->get_right());
	camera_->set_up(transform_->get_up());

	camera_->UpdateDirectionVectors();
}

void components::CameraComponent::Update()
{
	if (target1_ == nullptr || target2_ == nullptr)
	{
		camera_->view_matrix_ = glm::lookAt(camera_->position_, camera_->position_ + camera_->front_, camera_->up_);
	}
	else
	{
		glm::vec3 midPoint = calculateMidPoint() + glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 cameraPosition = glm::vec3(midPoint.x + distanceX_, midPoint.y + height_, midPoint.z + distanceZ_);
		camera_->set_position(glm::vec3(cameraPosition));

		glm::vec3 viewDir = glm::normalize(midPoint - camera_->get_position());

		camera_->yaw_ = glm::degrees(atan2(viewDir.z, viewDir.x));
		camera_->pitch_ = glm::degrees(atan2(viewDir.y, sqrt(viewDir.x * viewDir.x + viewDir.z * viewDir.z)));

		camera_->UpdateDirectionVectors();
		camera_->view_matrix_ = glm::lookAt(camera_->get_position(), camera_->get_position() + camera_->get_front(), camera_->get_up());
	}
}

void components::CameraComponent::Destroy()
{
}

components::CameraComponent::CameraComponent(s_ptr<GameObject> target1, s_ptr<GameObject> target2, s_ptr <llr::Camera> camera)
{

	target1_ = target1;
	target2_ = target2; 
	camera_ = camera;
	camera_->UpdateDirectionVectors();
}

components::CameraComponent::CameraComponent(s_ptr<llr::Camera> camera)
{
	camera_ = camera;
	target1_ = nullptr;
	target2_ = nullptr;
	camera_->UpdateDirectionVectors();
}

glm::vec3 components::CameraComponent::calculateMidPoint()
{
	return (target1_->transform_->get_position() + target2_->transform_->get_position()) / 2.0f;
}

void components::CameraComponent::SetTargets(s_ptr<GameObject> target1, s_ptr<GameObject> target2)
{
	target1_ = target1;
	target2_ = target2;
}

