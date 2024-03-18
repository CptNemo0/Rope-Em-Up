#ifndef TRANSFORM_H
#define TRANSFROM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <memory>

class Transform
{
private:
    Transform *parent_ = nullptr;
    bool is_dirty_ = false;
    std::vector<std::shared_ptr<Transform>> children_;

    glm::mat4 model_matrix_;

    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    void UpdateChildren();
    void CalculateModelMatrix(const glm::mat4 parent_model);

public:
    Transform();

    void AddChild(std::shared_ptr<Transform> child);

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_scale(const glm::vec3& scale);

    void translate(const glm::vec3& translation);
    const glm::mat4 get_model_matrix();
};

#endif // !TRANSFORM_H




