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

glm::vec3 llr::GameplayCamera::calculateMidPoint()
{
	glm::vec3 midPoint = glm::vec3(0.0f);
	midPoint = (target1_->transform_->get_position() + target2_->transform_->get_position()) / 2.0f;
	camera_->set_position(midPoint);
	return midPoint;
}

llr::GameplayCamera::GameplayCamera()
{
	camera_ = make_shared<Camera>();
	camera_->UpdateDirectionVectors();
}

llr::GameplayCamera::GameplayCamera(s_ptr <GameObject> target1, s_ptr <GameObject> target2, float pitch, float yaw, float fov, float near, float far, float ar)
{
	target1_ = target1;
	target2_ = target2;
	camera_ = make_shared<Camera>();
	camera_->set_position(calculateMidPoint());
	camera_->set_pitch(pitch);
	camera_->set_yaw(yaw);
	camera_->set_fov(fov);
	/*camera_->set_near(near);
	camera_->set_far(far);*/
	camera_->set_aspect_ratio(ar);
	camera_->UpdateDirectionVectors();
}


void components::GameplayCameraComponent::Start()
{
	glm::vec3 midPoint = GameplayCamera_->calculateMidPoint();
	transfrom_->set_position(midPoint);

}

void components::GameplayCameraComponent::Update()
{
	GameplayCamera_->camera_->set_position(transfrom_->get_position());
	GameplayCamera_->camera_->set_right(transfrom_->get_right());
	GameplayCamera_->camera_->set_up(transfrom_->get_up());
}
