#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "stb_easy_font.h"
#include <vector>

#include "../global.h"
#include "../GameObject.h"
#include "../components/Transform.h"

namespace components
{

class LegacyTextRenderer : public Component
{
private:
    unsigned int VAO_, VBO_, quads_count_;
    std::vector<float> vertices_buffer_;
    s_ptr<Shader> shader_;
    s_ptr<Transform> transform_;
public:
    LegacyTextRenderer(s_ptr<Shader> shader, string text);
    void ChangeText(string text);

    void Start() override;
    void Update() override;
    void Destroy() override {}
};

};

#endif // !TEXTRENDERER_H