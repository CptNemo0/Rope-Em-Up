#include "../../headers/components/ParticleEmitter.h"

components::ParticleEmitter::ParticleEmitter(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader)
{
    this->texture_ = texture;
    this->shader_ = shader;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    // BUFFERED FOR SINGLE PARTICLE
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * 1, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, size));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, color));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void components::ParticleEmitter::Start()
{
    transform_ = gameObject_.lock()->transform_;

    Particle particle;
    particle.position = transform_->get_position();
    particle.size = 1.0f;
    particle.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    particle.velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    particles_.push_back(particle);
}

void components::ParticleEmitter::Update()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id_);

    int particles_count = particles_.size();

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(Particle), particles_.data());

    glDrawArrays(GL_POINTS, 0, particles_count);
}

void components::ParticleEmitter::Destroy()
{
}
