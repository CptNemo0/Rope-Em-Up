#include "../headers/Transform.h"

Components::Transform::Transform()
{
    model_matrix_ = glm::mat4(1.0f);
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };
    scale_    = { 1.0f, 1.0f, 1.0f };
}

void Components::Transform::AddChild(std::shared_ptr<Transform> child)
{
    children_.push_back(child);
    child->parent_ = this;
    child->CalculateModelMatrix(model_matrix_);
}

const glm::vec3 Components::Transform::get_position() const
{
    return position_;
}

const glm::vec3 Components::Transform::get_previous_position() const
{
    return previous_position_;
}

const glm::vec3 Components::Transform::get_rotation() const
{
    return rotation_;
}

void Components::Transform::RemoveChild(std::shared_ptr<Transform> child)
{
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end())
    {
        children_.erase(it);
        child->parent_ = nullptr;
    }
}

const glm::vec3 Components::Transform::get_scale() const
{
    return scale_;
}

void Components::Transform::UpdateSelfAndChildren()
{
    CalculateModelMatrix(parent_ ? parent_->model_matrix_ : glm::mat4(1.0f));
    for (auto& child : children_)
    {
        child->UpdateSelfAndChildren();
    }
}

void Components::Transform::set_position(const glm::vec3 & position)
{
    previous_position_ = position_;
    position_ = position;
    predicted_position_ = position_;
    UpdateSelfAndChildren();
}

void Components::Transform::set_rotation(const glm::vec3 & rotation)
{
    rotation_ = rotation;
    UpdateSelfAndChildren();
}

void Components::Transform::set_scale(const glm::vec3 & scale)
{
    scale_ = scale;
    UpdateSelfAndChildren();
}

void Components::Transform::CalculateModelMatrix(const glm::mat4 parent_model)
{
    const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_);

    const glm::mat4 rotation_X = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_Y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation_Z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);

    model_matrix_ = translation * rotation_Y * rotation_X * rotation_Z * scale_matrix * parent_model;
}

const glm::mat4 Components::Transform::get_model_matrix()
{
    return model_matrix_;
}

void Components::Transform::add_position(const glm::vec3 & translation)
{
    previous_position_ = position_;
    position_ += translation;
    UpdateSelfAndChildren();
}

void Components::Transform::add_rotation(const glm::vec3 &rotation)
{
    rotation_ += rotation;
    UpdateSelfAndChildren();
}

void Components::Transform::add_scale(const glm::vec3 &scale)
{
    scale_ += scale;
    UpdateSelfAndChildren();
}

const glm::vec3 Components::Transform::get_predicted_position() const
{
    return predicted_position_;
}

void Components::Transform::set_predicted_position(const glm::vec3& position)
{
    predicted_position_ = position;
}

const glm::mat4 Components::Transform::get_prediction_matrix() const
{
    const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_);

    const glm::mat4 rotation_X = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_Y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation_Z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), predicted_position_);

    return translation * rotation_Y * rotation_X * rotation_Z * scale_matrix * parent_->get_model_matrix();
}