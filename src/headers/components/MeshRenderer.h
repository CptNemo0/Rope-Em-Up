#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "glad/glad.h"

#include <memory>
#include "nlohmann/json.hpp"

#include "../GameObject.h"
#include "../Model.h"
#include "../Texture.h"
#include "../res/Resources.h"
#include "../Shader.h"
#include "../components/Transform.h"

#include "../typedef.h"

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
		s_ptr<Shader> depth_shader_;

        glm::vec3 color_;

        void Start() override;
        void Update() override;
        void Destroy() override {}
        void Render(s_ptr<Shader> shader);

        MeshRenderer(json &j);
        json Serialize() override;
    };

};

#endif // !MESHRENDERER_H