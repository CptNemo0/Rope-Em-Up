#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <memory>

class GameObject;

namespace Components
{

class Transform
{
private:
    Transform *parent_ = nullptr;

    glm::mat4 model_matrix_;

    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    glm::vec3 previous_position_ = glm::vec3(0.0f);
    glm::vec3 predicted_position_ = glm::vec3(0.0f);

    void UpdateSelfAndChildren();
    void CalculateModelMatrix(const glm::mat4 parent_model);

public:
    std::shared_ptr<GameObject> game_object_;
    std::vector<std::shared_ptr<Transform>> children_;

    Transform();

    void AddChild(std::shared_ptr<Transform> child);

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
};

};

#endif // !TRANSFORM_H




