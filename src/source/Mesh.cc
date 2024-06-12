#include "../headers/Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool isRigged)
{
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->textures_ = textures;
    this->isRigged_ = isRigged;

    Init();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
}

void Mesh::Init()
{
    unsigned int vbo, ebo;

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    if (isRigged_)
    {
        glGenBuffers(1, &ssbo_);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_);
        glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BONES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_);
    }

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

    glBindVertexArray(0);
}


void Mesh::Draw(s_ptr<Shader> shader) const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_);
    for (unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string name = textures_[i].type_;
        if (name == "texture_albedo")
        {
            glUniform1i(glGetUniformLocation(shader->id_, "albedo_map"), i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
        }
        else if (name == "texture_normal")
        {
            glUniform1i(glGetUniformLocation(shader->id_, "normal_map"), i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
        }
        else if (name == "texture_metallic")
        {
            glUniform1i(glGetUniformLocation(shader->id_, "metallic_map"), i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
        }
        else if (name == "texture_roughness")
        {
            glUniform1i(glGetUniformLocation(shader->id_, "roughness_map"), i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
        }
        else if (name == "texture_emission")
        {
            glUniform1i(glGetUniformLocation(shader->id_, "emission_map"), i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
        }
    }


    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::UpdateTransforms(std::vector<glm::mat4> transforms)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, MAX_BONES * sizeof(glm::mat4), transforms.data());
}
