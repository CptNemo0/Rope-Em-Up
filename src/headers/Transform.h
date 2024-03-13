#ifndef TRANSFORM_H
#define TRANSFROM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform
{
private:
    glm::mat4 model_matrix_;

public:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    Transform();
    glm::mat4 get_model_matrix() const;
};

#endif // !TRANSFORM_H




