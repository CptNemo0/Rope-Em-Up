#include "headers/components/BillboardRenderer.h"
#include "headers/BillboardRendererManager.h"
components::BillboardRenderer::BillboardRenderer(s_ptr<tmp::Texture> texture, s_ptr<Shader> shader)
{
	texture_ = texture;
	shader_ = shader;

	this->texture_ = texture;
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void components::BillboardRenderer::Draw()
{
    shader_->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id_);

	shader_->SetMatrix4("model_matrix", transform_->get_local_model_matrix());

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    assert(transform_ != nullptr);
	assert(transform_->parent_ != nullptr);

	glm::vec3 pos_vec = transform_->parent_->get_global_position() + position_offset_;
	float pos[3] = {pos_vec.x, pos_vec.y, pos_vec.z};

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3, pos);

    glDrawArrays(GL_POINTS, 0, 1);
}

void components::BillboardRenderer::Start()
{
	transform_ = gameObject_.lock()->transform_;
}

void components::BillboardRenderer::Update()
{
}

void components::BillboardRenderer::Destroy()
{
	BillboardRendererManager::i_->RemoveRenderer(shared_from_this());
}


