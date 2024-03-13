#include "../headers/Mesh.h"

Mesh::Mesh(std::string mesh_path)
{
    vao_ = 0;
    vbo_ = 0;
    ebo_ = 0;

    indices_num_ = 0;

    vertices_ = std::vector<Vertex>();
    indices_ = std::vector<unsigned int>();

    scene_ = importer_.ReadFile(mesh_path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene_ == nullptr)
    {
        std::cout << "Failed to load a scene!!!\n";
        exit(1);
    }
    mesh_ = scene_->mMeshes[0];
    if (mesh_ == nullptr)
    {
        std::cout << "Failed to load mesh\n";
        exit(1);
    }

    for (int i = 0; i < mesh_->mNumVertices; i++)
    {
        Vertex new_vertex;
        new_vertex.position = glm::vec3(mesh_->mVertices[i].x, mesh_->mVertices[i].y, mesh_->mVertices[i].z);
        new_vertex.normal = glm::vec3(mesh_->mNormals[i].x, mesh_->mNormals[i].y, mesh_->mNormals[i].z);
        new_vertex.texture = glm::vec2(mesh_->mTextureCoords[0][i].x, mesh_->mTextureCoords[0][i].y);
        vertices_.push_back(new_vertex);
    }

    for (int i = 0; i < mesh_->mNumFaces; i++)
    {
        indices_.push_back(mesh_->mFaces[i].mIndices[0]);
        indices_.push_back(mesh_->mFaces[i].mIndices[1]);
        indices_.push_back(mesh_->mFaces[i].mIndices[2]);
    }

    indices_num_ = static_cast<unsigned int>(indices_.size());

    Init();
}

Mesh::~Mesh()
{
    const GLuint* arrays = new GLuint[1]{ vao_ };
    const GLuint* buffers = new GLuint[2]{vbo_, ebo_};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, arrays);
    delete[] arrays; 
    delete[] buffers;
}

void Mesh::Init()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
}

void Mesh::Draw() const
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_num_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
