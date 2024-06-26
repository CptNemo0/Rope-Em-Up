#include "../../headers/components/ParticleEmitter.h"

components::ParticleEmitter::ParticleEmitter(int max_particles, s_ptr<tmp::Texture> texture, s_ptr<Shader> shader, bool burst_emitter )
{
    this->texture_ = texture;
    this->shader_ = shader;
    this->max_particles_ = max_particles;
    this->burst_emitter_ = burst_emitter;
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

    emitter_timer_ = make_shared<Timer::Timer>(Timer::CreateTimer(pbd::kMsPerUpdate,
    [this]()
    {
        this->UpdateParticles(pbd::kMsPerUpdate);
    },
    nullptr, true));
    ParticleEmitterManager::i_->emitters_.push_back(shared_from_this());

    if (!burst_emitter_)
    {
        emission_timer_id_ = Timer::AddTimer(emission_rate_, [this]()
        {
            if (active_)
            {
                this->EmitParticles();
            }
        },
        nullptr, true);
    }
}

void components::ParticleEmitter::Update()
{
}

void components::ParticleEmitter::Destroy()
{
    particles_.clear();
    particle_indeces_to_remove_.clear();
    Timer::RemoveTimer(emission_timer_id_);
    ParticleEmitterManager::i_->RemoveEmitter(shared_from_this());
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void components::ParticleEmitter::UpdateParticles(float delta_time)
{
    auto view_matrix = Global::i_->active_camera_->GetViewMatrix();
    camera_distance_ = (view_matrix * glm::vec4(transform_->get_global_position(), 0.0f)).z;
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
    if (active_ && emit_particles_)
    {
        int rand_amount = random::RandInt(spawns_per_emission_.x, spawns_per_emission_.y);

        for (int i = 0; i < rand_amount; i++)
        {
            Particle particle;
            particle.life_time = microseconds(static_cast<int>(life_time_ * 1000000));
            particle.expiration_time = particle.life_time;
            particle.size = random::RandFloat(start_size_.x, start_size_.y);
            particle.color = start_color_;
            particle.position = transform_->get_global_position() + start_position_ + random::RandInsideSphere() * start_position_displacement_;
            particle.velocity = start_velocity_ + glm::vec3(random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_),
                                                            random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_),
                                                            random::RandFloat(-start_velocity_displacement_, start_velocity_displacement_));
            if (random_rotation_)
                particle.rotation_angle = random::RandFloat(0.0f, glm::two_pi<float>());
            else
                particle.rotation_angle = 0.0f;
            particle.id_ = current_id_++;
            particles_.push_back(particle);
        }
    }
}

void components::ParticleEmitter::Burst(int amount)
{
    if (active_)
    {
        for (int i = 0; i < amount; i++)
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
            if (random_rotation_)
                particle.rotation_angle = 0.0f;// random::RandFloat(0.0f, glm::two_pi<float>());
            else
                particle.rotation_angle = 0.0f;
            particle.id_ = current_id_++;
            particles_.push_back(particle);
        }
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

components::ParticleEmitter::ParticleEmitter(json &j)
{
    int max_particles = j["max_particles"];
    s_ptr<tmp::Texture> texture = res::get_texture(j["texture_path"]);
    s_ptr<Shader> shader = res::get_shader(j["shader_paths"][0], j["shader_paths"][1], j["shader_paths"][2]);

    this->ParticleEmitter::ParticleEmitter(max_particles, texture, shader);
    emit_particles_ = j["emit_particles"];
    emission_rate_ = j["emission_rate"];
    spawns_per_emission_ = {j["spawns_per_emission"][0], j["spawns_per_emission"][1]};
    life_time_ = j["life_time"];
    start_position_ = {j["start_position"][0], j["start_position"][1], j["start_position"][2]};
    start_size_ = {j["start_size"][0], j["start_size"][1]};
    end_size_ = {j["end_size"][0], j["end_size"][1]};
    start_color_ = {j["start_color"][0], j["start_color"][1], j["start_color"][2], j["start_color"][3]};
    end_color_ = {j["end_color"][0], j["end_color"][1], j["end_color"][2], j["end_color"][3]};
    start_position_displacement_ = j["start_position_displacement"];
    start_velocity_ = {j["start_velocity"][0], j["start_velocity"][1], j["start_velocity"][2]};
    start_velocity_displacement_ = j["start_velocity_displacement"];
    start_acceleration_ = {j["start_acceleration"][0], j["start_acceleration"][1], j["start_acceleration"][2]};
    burst_emitter_ = j["burst_emitter_"];
}

json components::ParticleEmitter::Serialize()
{
    json j;

    j["max_particles"] = max_particles_;
    j["texture_path"] = texture_->path_;
    j["shader_paths"] = {shader_->v_path, shader_->g_path, shader_->f_path};

    j["emit_particles"] = emit_particles_;
    j["emission_rate"] = emission_rate_;
    j["spawns_per_emission"] = {spawns_per_emission_.x, spawns_per_emission_.y};
    j["life_time"] = life_time_;
    j["start_position"] = {start_position_.x, start_position_.y, start_position_.z};
    j["start_size"] = {start_size_.x, start_size_.y};
    j["end_size"] = {end_size_.x, end_size_.y};
    j["start_color"] = {start_color_.r, start_color_.g, start_color_.b, start_color_.a};
    j["end_color"] = {end_color_.r, end_color_.g, end_color_.b, end_color_.a};
    j["start_position_displacement"] = start_position_displacement_;
    j["start_velocity"] = {start_velocity_.x, start_velocity_.y, start_velocity_.z};
    j["start_velocity_displacement"] = start_velocity_displacement_;
    j["start_acceleration"] = {start_acceleration_.x, start_acceleration_.y, start_acceleration_.z};
    j["burst_emitter_"] = burst_emitter_;
    return j;
}
