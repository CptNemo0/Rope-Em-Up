#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "stb_easy_font.h"
#include <vector>

#include "GameObject.h"
#include "Transform.h"

namespace Components
{

class TextRenderer : public Component
{
private:
    unsigned int VAO_, VBO_, quads_count_;
    std::vector<float> vertices_buffer_;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<Transform> transform_;
public:
    TextRenderer(std::shared_ptr<Shader> shader, std::string text);
    void ChangeText(std::string text);

    void Start() override;
    void Update() override;
};

};

#endif // !TEXTRENDERER_H