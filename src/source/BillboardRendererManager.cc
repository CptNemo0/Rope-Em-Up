#include "../headers/BillboardRendererManager.h"

BillboardRendererManager* BillboardRendererManager::i_ = nullptr;

BillboardRendererManager::BillboardRendererManager()
{
	shader_ = res::get_shader("res/shaders/Billboard.vert", "res/shaders/Billboard.frag");
}

std::shared_ptr<components::BillboardRenderer> BillboardRendererManager::CreateRenderer(std::shared_ptr<Model> model, std::shared_ptr<tmp::Texture> texture)
{
	std::shared_ptr<components::BillboardRenderer> renderer = std::make_shared<components::BillboardRenderer>(model, texture);
	AddRenderer(renderer);
	return renderer;
}

void BillboardRendererManager::AddRenderer(std::shared_ptr<components::BillboardRenderer> r)
{
	renderers_.push_back(r);
}

void BillboardRendererManager::RemoveRenderer(std::shared_ptr<components::BillboardRenderer> r)
{
	auto it = std::find(renderers_.begin(), renderers_.end(), r);
	if (it != renderers_.end())
	{
		renderers_.erase(it);
	}
}

void BillboardRendererManager::UpdateRenderers()
{
	shader_->Use();
	shader_->SetMatrix4("view_matrix", Global::i_->active_camera_->GetViewMatrix());
	for (auto& render : renderers_)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render->texture_->id_);
		shader_->SetInt("albedo", 0);

		//auto transfrom = render->gameObject_.lock()->transform_;

		//glm::vec3 A = -1.0f * glm::normalize(Global::i_->active_camera_->get_position() - transfrom->get_position());
		//glm::vec3 B = transfrom->get_up();

		//glm::vec3 axis = glm::cross(A, B);
		//float angle = acos(glm::dot(A, B));
		//glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, axis);
		//glm::vec3 euler_angles;
		//euler_angles.y = glm::degrees(asin(rotation_matrix[0][2])); // Pitch (Y-axis rotation)
		//euler_angles.x = glm::degrees(atan2(-rotation_matrix[1][2], rotation_matrix[2][2])); // Roll (X-axis rotation)
		//euler_angles.z = glm::degrees(atan2(-rotation_matrix[0][1], rotation_matrix[0][0]));

		//cout << euler_angles.x << " " << euler_angles.y << " " << euler_angles.z << endl;

		//render->gameObject_.lock()->transform_->set_rotation(euler_angles);

		shader_->SetMatrix4("model_matrix", render->gameObject_.lock()->transform_->get_model_matrix());
		render->Draw();
	}
}
