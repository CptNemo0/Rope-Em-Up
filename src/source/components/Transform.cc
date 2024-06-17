#include "../../headers/components/Transform.h"

components::Transform::Transform()
{
    model_matrix_ = glm::mat4(1.0f);
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };
    scale_    = { 1.0f, 1.0f, 1.0f };
    forward_ = { 0.0f, 0.0f, 1.0f };
    right_ = { 1.0f, 0.0f, 0.0f };
    up_ = { 0.0f, 1.0f, 0.0f };
}

void components::Transform::AddChild(s_ptr<Transform> child)
{
    children_.push_back(child);
    child->parent_ = this;
    child->CalculateModelMatrix(model_matrix_);
}

const glm::vec3 components::Transform::get_position() const
{
    return position_;
}

const glm::vec3 components::Transform::get_global_position() const
{
    return glm::vec3(model_matrix_[3]);
}

const glm::vec3 components::Transform::get_previous_position() const
{
    return previous_position_;
}

const glm::vec3 components::Transform::get_rotation() const
{
    return rotation_;
}

void components::Transform::RemoveChild(s_ptr<Transform> child)
{
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end())
    {
        children_.erase(it);
        child->parent_ = nullptr;
    }
}

const glm::vec3 components::Transform::get_scale() const
{
    return scale_;
}

void components::Transform::UpdateSelfAndChildren()
{
    CalculateModelMatrix(parent_ ? parent_->model_matrix_ : glm::mat4(1.0f));
    for (auto& child : children_)
    {
        child->UpdateSelfAndChildren();
    }
}

void components::Transform::set_position(const glm::vec3 & position)
{
    previous_position_ = position_;
    position_ = position;
    predicted_position_ = position_;

    UpdateSelfAndChildren();
}

void components::Transform::set_rotation(const glm::vec3 & rotation)
{
    rotation_ = rotation;
    UpdateSelfAndChildren();
}

void components::Transform::set_scale(const glm::vec3 & scale)
{
    scale_ = scale;
    UpdateSelfAndChildren();
}

void components::Transform::scale_in(const glm::vec3 &direction, float amount)
{
    position_ += direction * ((amount - 1.0f) * scale_);
    scale_ *= glm::abs(direction) * amount + (glm::vec3(1.0f) - glm::abs(direction));
    UpdateSelfAndChildren();
}

void components::Transform::scale(const glm::vec3 &scale)
{
    scale_ *= scale;
    UpdateSelfAndChildren();
}

void components::Transform::CalculateModelMatrix(const glm::mat4 parent_model)
{
    const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_);

    const glm::mat4 rotation_X = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_Y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation_Z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);

    model_matrix_ = parent_model * translation * rotation_Y * rotation_X * rotation_Z * scale_matrix;

    auto up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    auto forward = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    auto right = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    right_ = (rotation_Y * right);
    forward_ = (rotation_Y * forward);
}

const glm::mat4 components::Transform::get_model_matrix()
{
    return model_matrix_;
}

void components::Transform::add_position(const glm::vec3 & translation)
{
    previous_position_ = position_;
    position_ += translation;
    UpdateSelfAndChildren();
}

void components::Transform::add_rotation(const glm::vec3 &rotation) 
{
    rotation_ += rotation;
    UpdateSelfAndChildren();   
}

void components::Transform::add_scale(const glm::vec3 &scale)
{
    scale_ += scale;
    UpdateSelfAndChildren();
}

const glm::vec3 components::Transform::get_predicted_position() const
{
    return predicted_position_;
}

void components::Transform::set_predicted_position(const glm::vec3& position)
{
    predicted_position_ = position;
}

void components::Transform::set_previous_position(const glm::vec3& position)
{
    previous_position_ = position;
}

const glm::mat4 components::Transform::get_prediction_matrix() const
{
    const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_);

    const glm::mat4 rotation_X = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_Y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation_Z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), predicted_position_);

    return parent_->get_model_matrix() * translation * rotation_Y * rotation_X * rotation_Z * scale_matrix;
}

const glm::vec3 components::Transform::get_up() const
{
    return up_;
}

const glm::vec3 components::Transform::get_forward() const
{
    return forward_;
}

const glm::vec3 components::Transform::get_right() const
{
    return right_;
}

void components::Transform::TeleportToPosition(glm::vec3 position)
{
    previous_position_ = position;
    position_ = position;
    predicted_position_ = position;
    UpdateSelfAndChildren();
}

void components::Transform::FixOrientation()
{
    auto current_forward = get_position() - get_predicted_position();
    if (glm::length(current_forward))
    {
        current_forward = glm::normalize(current_forward);
    }
    float angle = glm::degrees(glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), current_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    set_rotation(glm::vec3(0.0f, angle, 0.0f));
}

void components::Transform::set_from_json(json &j)
{
    position_ = glm::vec3(j["position"][0], j["position"][1], j["position"][2]);
    rotation_ = glm::vec3(j["rotation"][0], j["rotation"][1], j["rotation"][2]);
    scale_ = glm::vec3(j["scale"][0], j["scale"][1], j["scale"][2]);

    previous_position_ = glm::vec3(j["previous_position"][0], j["previous_position"][1], j["previous_position"][2]);
    predicted_position_ = glm::vec3(j["predicted_position"][0], j["predicted_position"][1], j["predicted_position"][2]);

    UpdateSelfAndChildren();
}

json components::Transform::Serialize()
{
    json j;

    j["position"] = { position_.x, position_.y, position_.z };
    j["rotation"] = { rotation_.x, rotation_.y, rotation_.z };
    j["scale"] = { scale_.x, scale_.y, scale_.z };

    j["previous_position"] = { previous_position_.x, previous_position_.y, previous_position_.z };
    j["predicted_position"] = { predicted_position_.x, predicted_position_.y, predicted_position_.z };

    return j;
}
