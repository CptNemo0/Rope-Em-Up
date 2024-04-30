#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "glad/glad.h"

#include <memory>

#include "../global.h"
#include "../GameObject.h"
#include "../Model.h"
#include "../Texture.h"

#include "../Shader.h"
#include "../components/Transform.h"

namespace components
{

    class MeshRenderer : public Component
    {
    public:

        MeshRenderer(s_ptr<Model> model,
                    s_ptr<Shader> shader);

        s_ptr<Transform> transform_;
        s_ptr<Model> model_;
        s_ptr<Shader> shader_;

        void Start() override;
        void Update() override;
        void Destroy() override {}
        void Render(s_ptr<Shader> shader);
    };

};

#endif // !MESHRENDERER_H