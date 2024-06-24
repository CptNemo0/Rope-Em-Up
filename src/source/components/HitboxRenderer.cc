#include "../../headers/components/HitboxRenderer.h"
#include "../../headers/HitboxManager.h"
components::HitboxRenderer::HitboxRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt)
{
	float random_y_offset = random::RandFloat(-0.0001f, 0.0001);
	this->vertices_data_[ 0] = lb.x;
	this->vertices_data_[ 1] = 0.3f + random_y_offset;
	this->vertices_data_[ 2] = lb.z;
					   // 3
	                   // 4
	this->vertices_data_[ 5] = rb.x;
	this->vertices_data_[ 6] = 0.3f + random_y_offset;
	this->vertices_data_[ 7] = rb.z;
					   // 8
	                   // 9
	this->vertices_data_[10] = rt.x;
	this->vertices_data_[11] = 0.3f + random_y_offset;
	this->vertices_data_[12] = rt.z;
	                   //13
	                   //14
	this->vertices_data_[15] = lt.x;
	this->vertices_data_[16] = 0.3f + random_y_offset;
	this->vertices_data_[17] = lt.z;

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data_), &vertices_data_, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_data_), &indices_data_, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	this->texture_ = res::get_texture("res/indicators/hitbox.png");
	this->color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
}

void components::HitboxRenderer::Draw()
{
	
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void components::HitboxRenderer::Start()
{
}

void components::HitboxRenderer::Update()
{
}

void components::HitboxRenderer::Destroy()
{
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);

	HitboxManager::i_->RemoveRenderer(shared_from_this());
}
