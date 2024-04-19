#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../Font.h"

#include "../GameObject.h"

namespace components
{

class TextRenderer : public Component
{
private:
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<Transform> transform_;
    unsigned int VAO_, VBO_;
public:
    std::string text_;
    glm::vec3 color_;
    std::shared_ptr<Font> font_;

    TextRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<Font> font, std::string text, glm::vec3 color = glm::vec3(0.0f));

    void Start() override;
    void Update() override;
    void Destroy() override {}
};

}

#endif // !TEXTRENDERER_H