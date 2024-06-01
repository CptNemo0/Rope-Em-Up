#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include <memory>
#include "nlohmann/json.hpp"

#include "../GameObject.h"
#include "../Texture.h"
#include "../res/Resources.h"

#include "../global.h"

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
    s_ptr<Shader> shader_;
    std::weak_ptr<Transform> transform_;
public:
    s_ptr<tmp::Texture> texture_;

    HUDRenderer(s_ptr<tmp::Texture> texture, s_ptr<Shader> shader);
    void Start() override;
    void Update() override;
    void Destroy() override {}

    HUDRenderer(json &j);
    json Serialize() override;
};

}; // namespace Components

#endif // !HUDRENDERER_H