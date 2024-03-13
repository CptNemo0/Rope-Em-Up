#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "headers/GameObject.h"
#include "headers/Mesh.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"

#include <ctime>
#include <ratio>
#include <chrono>

const std::string kWindowTitle = "Modul Sumatywny";

const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";

const std::string kGreenTexturePath = "res/textures/green_texture.png";
const std::string kRedTexturePath = "res/textures/red_texture.png";

const std::string kCubeMeshPath = "res/models/cube.obj";

const glm::vec3 kCameraPosition = glm::vec3(0.0f, 5.0f, -5.0f);

const glm::vec3 kRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 kUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 kForward = glm::vec3(0.0f, 0.0f, 1.0f);

int main()
{
    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    GLFWvidmode* mode = nullptr;

    if (int return_value = utility::InitGLFW(window, monitor, mode, kWindowTitle))
    {
        exit(return_value);
    }
    std::cout << "GLFW Initialized.\n";

    if (int return_value = utility::InitGlad())
    {
        exit(return_value);
    }
    std::cout << "GLAD Initialized.\n";

    auto shader = std::make_shared<Shader>(kVertexShaderPath, kFragmentShaderPath);

    auto green_texture = std::make_shared<Texture>(kGreenTexturePath);
    auto red_texture = std::make_shared<Texture>(kRedTexturePath);

    auto cube_mesh = std::make_shared<Mesh>(kCubeMeshPath);

    auto object = std::make_shared<GameObject>(cube_mesh, green_texture, shader);
    auto object2 = std::make_shared<GameObject>(cube_mesh, red_texture, shader);

    auto view_matrix = glm::lookAt(kCameraPosition, object->transform_.position_, kUp);
    auto projection_matrix = glm::perspective(glm::radians(90.0f), (1920.0f / 1080.0f), 0.1f, 1000.0f);

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.1f, 0.1f, 0.1f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    object->transform_.position_.x -= 2;
    object2->transform_.position_.x += 2;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.03f, 0.04f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();

        shader->SetPointLight("light", point_light);
        shader->SetMatrix4("projection_matrix", projection_matrix);
        shader->SetMatrix4("view_matrix", view_matrix);

        object->Render();
        object2->Render();

        glfwSwapBuffers(window);
    }

    shader->End();
    glfwTerminate();
    return 0;
}