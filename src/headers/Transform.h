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
    bool is_dirty_ = false;

    glm::mat4 model_matrix_;

    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    void UpdateChildren();
    void CalculateModelMatrix(const glm::mat4 parent_model);

public:
    GameObject *game_object_ = nullptr;
    std::vector<std::shared_ptr<Transform>> children_;

    Transform(GameObject *game_object);

    void AddChild(std::shared_ptr<Transform> child);

    const glm::vec3 get_position() const;
    const glm::vec3 get_rotation() const;
    const glm::vec3 get_scale() const;

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_scale(const glm::vec3& scale);

    void translate(const glm::vec3& translation);
    const glm::mat4 get_model_matrix();
};

};

#endif // !TRANSFORM_H




