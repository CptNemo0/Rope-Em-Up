#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "glad/glad.h"

#include <memory>


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

        MeshRenderer(std::shared_ptr<Model> model,
                    std::shared_ptr<Shader> shader);

        std::shared_ptr<Transform> transform_;
        std::shared_ptr<Model> model_;
        std::shared_ptr<Shader> shader_;

        void Start() override;
        void Update() override;
        void Destroy() override {}
    };

};

#endif // !MESHRENDERER_H