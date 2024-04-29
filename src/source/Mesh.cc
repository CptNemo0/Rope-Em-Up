#include "../headers/Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->textures_ = textures;

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
    glBindVertexArray(0);
}


void Mesh::Draw(s_ptr<Shader> shader) const
{
    unsigned int albedoeNr = 0;
    unsigned int normalNr = 0;
    unsigned int metalicNr = 0;
    unsigned int roughnesstNr = 0;
    /*for (unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures_[i].type_;
        if (name == "texture_albedo")
            number = std::to_string(albedoeNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_metallic")
            number = std::to_string(metalicNr++);
        else if (name == "texture_roughness")
            number = std::to_string(roughnesstNr++);

        glUniform1i(glGetUniformLocation(shader->id_, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
    }*/

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
    }


    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
