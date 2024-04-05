#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Transform> transform, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader)
{
    this->transform_ = transform;
	this->mesh_ = mesh;
	this->material_ = material;
	this->shader_ = shader;
}

void Components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	/*glBindTexture(GL_TEXTURE_2D, texture_->id_);*/

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < material_->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = material_->textures.at(i)->type_;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader_->id_, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, material_->textures.at(i)->id_);
    }

	mesh_->Draw();
}
