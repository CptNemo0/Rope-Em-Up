#ifndef PARTICLEEMITTER_H
#define PARTICICLEEMITTER_H

#include "../GameObject.h"
#include "../Texture.h"

namespace components
{

class ParticleEmitter : public Component
{
private:
    std::shared_ptr<tmp::Texture> texture;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Transform> transform;

    unsigned int VAO_, VBO_;
public:
    ParticleEmitter(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader);
    void Start() override;
    void Update() override;
    void Destroy() override;
};

} // namespace components

#endif // PARTICLEEMITTER_H