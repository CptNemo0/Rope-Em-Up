#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <chrono>
#include <deque>
#include <unordered_set>
#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "../GameObject.h"
#include "../Texture.h"
#include "../Timer.h"
#include "../physics/PBD.h"
#include "../ParticleEmitterManager.h"
#include "../Camera.h"
#include "../Random.h"

#include "../typedef.h"
#include "../Global.h"

struct Particle
{
    glm::vec3 position;
    float size;
    glm::vec4 color;
    float rotation_angle;

    glm::vec3 velocity;
    ::microseconds expiration_time;
    ::microseconds life_time;

    float camera_distance;

    unsigned int id_;
};

namespace components
{

class ParticleEmitter : public Component, public std::enable_shared_from_this<ParticleEmitter>
{
private:
    int max_particles_;
    
    s_ptr<Shader> shader_;
    s_ptr<Transform> transform_;

    std::deque<Particle> particles_;
    std::unordered_set<int> particle_indeces_to_remove_;

    unsigned int VAO_, VBO_;

    unsigned int current_id_ = 0;
public:
    float camera_distance_ = 0.0f;

    s_ptr<tmp::Texture> texture_;

    bool emit_particles_ = true;
    float emission_rate_ = 0.05f;
    glm::ivec2 spawns_per_emission_ = glm::ivec2(1, 1);
    float life_time_ = 1.0f;
    glm::vec3 start_position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 start_size_ = glm::vec2(1.0f, 1.0f);
    glm::vec2 end_size_ = glm::vec2(0.0f, 0.0f);
    glm::vec4 start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    float start_position_displacement_ = 0.1f;
    glm::vec3 start_velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
    float start_velocity_displacement_ = 0.0f;
    glm::vec3 start_acceleration_ = glm::vec3(0.0f, 0.0f, 0.0f);

    bool burst_emitter_;

    s_ptr<Timer::Timer> emitter_timer_;
    Timer::Timer emission_timer_;

    ParticleEmitter(int max_particles, s_ptr<tmp::Texture> texture, s_ptr<Shader> shader, bool burst_emitter = false);
   
    void Start() override;
    void Update() override;
    void Destroy() override;

    void UpdateParticles(float delta_time);
    void EmitParticles();
    void Burst(int amount);
    void DrawParticles();

    ParticleEmitter(json &j);
    json Serialize() override;
};

} // namespace components

#endif // PARTICLEEMITTER_H