#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "GLFW/glfw3.h"
#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "../Font.h"
#include "../GameObject.h"
#include "../global.h"

namespace components
{

class TextRenderer : public Component
{
private:
    s_ptr<Shader> shader_;
    s_ptr<Transform> transform_;
    unsigned int VAO_, VBO_;
public:
    string text_;
    glm::vec3 color_;
    s_ptr<Font> font_;

    TextRenderer(s_ptr<Shader> shader, s_ptr<Font> font, string text, glm::vec3 color = glm::vec3(0.0f));

    void Start() override;
    void Update() override;
    void Destroy() override {}

    TextRenderer(json &j);
    json Serialize() override;
};

}

#endif // !TEXTRENDERER_H