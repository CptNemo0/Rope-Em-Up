#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "glad/glad.h"

#include <memory>

#include "Component.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

namespace Components
{

class MeshRenderer : public Component
{
public:
    MeshRenderer(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model,
                 std::shared_ptr<Shader> shader);

    std::shared_ptr<Transform> transform_;
    std::shared_ptr<Model> model_;
	std::shared_ptr<Shader> shader_;

    void Start() override {};
    void Update() override;
};

};

#endif // !MESHRENDERER_H