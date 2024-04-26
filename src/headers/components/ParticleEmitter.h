#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <chrono>
#include <deque>
#include <unordered_set>

#include "../GameObject.h"
#include "../Texture.h"
#include "../Timer.h"
#include "../physics/PBD.h"
#include "../ParticleEmitterManager.h"
#include "../Camera.h"

struct Particle
{
    glm::vec3 position;
    float size;
    glm::vec4 color;
    float rotation_angle;

    glm::vec3 velocity;
    std::chrono::microseconds expiration_time;
    std::chrono::microseconds life_time;

    float camera_distance;

    unsigned int id_;
};

namespace components
{

class ParticleEmitter : public Component, public std::enable_shared_from_this<ParticleEmitter>
{
private:
    const int MAX_PARTICLES = 1000000;

    std::shared_ptr<tmp::Texture> texture_;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<llr::Camera> camera_;
    std::shared_ptr<Transform> transform_;

    std::deque<Particle> particles_;
    std::unordered_set<int> particle_indeces_to_remove_;

    unsigned int VAO_, VBO_;

    std::shared_ptr<Timer::Timer> emitter_timer_;

    unsigned int current_id_ = 0;
public:
    bool active_ = true;
    float emission_rate_ = 0.05f;
    glm::ivec2 spawns_per_emission_ = glm::ivec2(1, 1);
    float life_time_ = 1.0f;
    glm::vec2 start_size_ = glm::vec2(1.0f, 1.0f);
    glm::vec2 end_size_ = glm::vec2(0.0f, 0.0f);
    glm::vec4 start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    float start_position_displacement_ = 0.1f;
    glm::vec3 start_velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
    float start_velocity_displacement_ = 0.0f;
    glm::vec3 start_acceleration_ = glm::vec3(0.0f, 0.0f, 0.0f);

    ParticleEmitter(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader, std::shared_ptr<llr::Camera> camera);
    void Start() override;
    void Update() override;
    void Destroy() override;

    void UpdateParticles(float delta_time);
    void EmitParticles();
    void DrawParticles();

    // inclusive
    static inline float rand_float(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    // inclusive
    static inline int rand_int(int min, int max)
    {
        return min + rand() % (max - min + 1);
    }

    // unit sphere uniform distribution
    static inline glm::vec3 rand_sphere_vec3()
    {
        float u = rand_float(0.0f, 1.0f);
        float v = rand_float(0.0f, 1.0f);
        float theta = u * 2.0f * glm::pi<float>();
        float phi = glm::acos(2.0f * v - 1.0f);
        float r = std::cbrtf(rand_float(0.0f, 1.0f));
        float sin_theta = glm::sin(theta);
        float cos_theta = glm::cos(theta);
        float sin_phi = glm::sin(phi);
        float cos_phi = glm::cos(phi);
        return glm::vec3(r * sin_phi * cos_theta, r * sin_phi * sin_theta, r * cos_phi);
    }
};

} // namespace components

#endif // PARTICLEEMITTER_H