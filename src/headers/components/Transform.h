#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

#include <vector>
#include <memory>

class GameObject;

namespace components
{

class Transform
{
private:
    glm::mat4 model_matrix_;

    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    glm::vec3 up_;
    glm::vec3 forward_;
    glm::vec3 right_;

    glm::vec3 previous_position_ = glm::vec3(0.0f);
    glm::vec3 predicted_position_ = glm::vec3(0.0f);

    void UpdateSelfAndChildren();
    void CalculateModelMatrix(const glm::mat4 parent_model);

public:
    std::shared_ptr<GameObject> game_object_;
    std::vector<std::shared_ptr<Transform>> children_;
    Transform *parent_ = nullptr;

    Transform();

    void AddChild(std::shared_ptr<Transform> child);
    void RemoveChild(std::shared_ptr<Transform> child);

    const glm::vec3 get_position() const;
    const glm::vec3 get_previous_position() const;
    const glm::vec3 get_predicted_position() const;
    const glm::vec3 get_rotation() const;
    const glm::vec3 get_scale() const;

    void set_position(const glm::vec3& position);
    void set_predicted_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_scale(const glm::vec3& scale);

    void add_position(const glm::vec3& translation);
    void add_rotation(const glm::vec3& rotation);
    void add_scale(const glm::vec3& scale);
    const glm::mat4 get_model_matrix();
    const glm::mat4 get_prediction_matrix() const;

    const glm::vec3 get_up() const;
    const glm::vec3 get_forward() const;
    const glm::vec3 get_right() const;

    void FixOrientation();
};

};

#endif // !TRANSFORM_H




