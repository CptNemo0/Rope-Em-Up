#include "../../headers/components/ParticleEmitter.h"

components::ParticleEmitter::ParticleEmitter(int max_particles, s_ptr<tmp::Texture> texture, s_ptr<Shader> shader, s_ptr<llr::Camera> *camera)
{
    this->texture_ = texture;
    this->shader_ = shader;
    this->camera_ = camera;
    this->max_particles_ = max_particles;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    // BUFFERED FOR MAX PARTICLES
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * max_particles_, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, size));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, rotation_angle));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void components::ParticleEmitter::Start()
{
    transform_ = gameObject_.lock()->transform_;

    this->emitter_timer_ = make_shared<Timer::Timer>(Timer::CreateTimer(pbd::kMsPerUpdate,
    [this]()
    {
        this->UpdateParticles(pbd::kMsPerUpdate);
    },
    nullptr, true));
    ParticleEmitterManager::i_->emitter_timers_.push_back(emitter_timer_);
    ParticleEmitterManager::i_->emitters_.push_back(shared_from_this());

    Timer::AddTimer(emission_rate_, [this]()
    {
        if (active_)
        {
            this->EmitParticles();
        }
    },
    nullptr, true);
}

void components::ParticleEmitter::Update()
{
}

void components::ParticleEmitter::Destroy()
{
}

void components::ParticleEmitter::UpdateParticles(float delta_time)
{
    auto view_matrix = (*camera_)->GetViewMatrix();
    float inverse_life_time = 1.0f / static_cast<float>(microseconds(static_cast<int>(life_time_ * 1000000)).count());
    for (auto &particle : particles_)
    {
        particle.expiration_time -= microseconds(static_cast<int>(delta_time * 1000000));
        if (particle.expiration_time < microseconds(0))
        {
            particle.expiration_time = microseconds(0);
            particle_indeces_to_remove_.insert(particle.id_);
        }
        particle.position += particle.velocity * delta_time;
        particle.velocity += start_acceleration_ * delta_time;
        float t = 1.0f - (particle.expiration_time.count() * inverse_life_time);
        particle.color = glm::mix(start_color_, end_color_, t);
        particle.size = glm::mix(start_size_.x, end_size_.x, t);
        particle.camera_distance = (view_matrix * glm::vec4(particle.position, 0.0f)).z;
    }

    std::erase_if(particles_, [this](const Particle &p)
    {
        return particle_indeces_to_remove_.find(p.id_) != particle_indeces_to_remove_.end();
    });
    particle_indeces_to_remove_.clear();

    std::sort(particles_.begin(), particles_.end(), [](const Particle &a, const Particle &b)
    {
        return a.camera_distance < b.camera_distance;
    });
}

void components::ParticleEmitter::EmitParticles()
{
    int rand_amount = random::RandInt(spawns_per_emission_.x, spawns_per_emission_.y);

    for (int i = 0; i < rand_amount; i++)
    {
        Particle particle;
        particle.life_time = microseconds(static_cast<int>(life_time_ * 1000000));
        particle.expiration_time = particle.life_time;
        particle.size = random::RandFloat(start_size_.x, start_size_.y);
        particle.color = start_color_;
        particle.position = transform_->get_global_position() + random::RandInsideSphere() * start_position_displacement_;
        particle.velocity = start_velocity_ + glm::vec3(random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_),
                                                        random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_),
                                                        random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_));
        particle.rotation_angle = random::RandFloat(0.0f, glm::two_pi<float>());
        particle.id_ = current_id_++;
        particles_.push_back(particle);
    }
}

void components::ParticleEmitter::DrawParticles()
{
    shader_->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id_);

    int particles_count = particles_.size();

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    std::vector<Particle> particle_data(particles_.begin(), particles_.end());

    glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(Particle), particle_data.data());

    glDrawArrays(GL_POINTS, 0, particles_count);
}
