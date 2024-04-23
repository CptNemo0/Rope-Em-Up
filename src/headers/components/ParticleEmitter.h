#ifndef PARTICLEEMITTER_H
#define PARTICICLEEMITTER_H

#include "../GameObject.h"
#include "../Texture.h"

struct Particle
{
    glm::vec3 position;
    float size;
    glm::vec4 color;

    glm::vec3 velocity;
};

namespace components
{

class ParticleEmitter : public Component
{
private:
    const int MAX_PARTICLES = 1000;

    std::shared_ptr<tmp::Texture> texture_;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<Transform> transform_;

    std::vector<Particle> particles_;

    unsigned int VAO_, VBO_;
public:
    ParticleEmitter(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader);
    void Start() override;
    void Update() override;
    void Destroy() override;
};

} // namespace components

#endif // PARTICLEEMITTER_H