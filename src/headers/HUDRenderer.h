#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include <memory>

#include "GameObject.h"
#include "Texture.h"

class HUDRenderer : public Component
{
private:
    static inline const float vertices_[8] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
    };
    unsigned int VAO_, VBO_;
    std::shared_ptr<Texture> texture_;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<Components::Transform> transform_;
    glm::vec3 internal_scale_;
public:
    HUDRenderer(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader);
    void Start() override;
    void Update() override;
};

#endif // !HUDRENDERER_H