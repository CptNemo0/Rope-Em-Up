#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include <memory>

#include "../GameObject.h"
#include "../Texture.h"

namespace components
{

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
    std::shared_ptr<tmp::Texture> texture_;
    std::shared_ptr<Shader> shader_;
    std::weak_ptr<Transform> transform_;
public:
    HUDRenderer(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader);
    void Start() override;
    void Update() override;
    void Destroy() override {}
};

}; // namespace Components

#endif // !HUDRENDERER_H