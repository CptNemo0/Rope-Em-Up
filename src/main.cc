#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "headers/Camera.h"
#include "headers/GameObject.h"
#include "headers/Mesh.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"

#include <ctime>
#include <ratio>
#include <chrono>

int main()
{
    const std::string kWindowTitle = "Modul Sumatywny";

    const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
    const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";

    const std::string kGreenTexturePath = "res/textures/green_texture.png";
    const std::string kRedTexturePath = "res/textures/red_texture.png";

    const std::string kCubeMeshPath = "res/models/cube.obj";

    const float kFov = 90.0f;
    const float kNear = 0.1f;
    const float kFar = 1000.0f;

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
    
    auto camera = std::make_shared<llr::Camera>();
    camera->set_fov(kFov);
    camera->set_near(kNear);
    camera->set_far(kFar);
    camera->set_aspect_ratio(((float)mode->width / (float)mode->height));

    auto shader = std::make_shared<Shader>(kVertexShaderPath, kFragmentShaderPath);

    auto green_texture = std::make_shared<Texture>(kGreenTexturePath);
    auto red_texture = std::make_shared<Texture>(kRedTexturePath);

    auto cube_mesh = std::make_shared<Mesh>(kCubeMeshPath);
    
    auto object = std::make_shared<GameObject>(cube_mesh, green_texture, shader);
    auto object2 = std::make_shared<GameObject>(cube_mesh, red_texture, shader);

    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());

    PointLight point_light;
    point_light.intensity = 50.0f;
    point_light.position = glm::vec3(0.0f, 10.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.2f, 0.2f, 0.2f);
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
        shader->SetMatrix4("view_matrix", camera->GetViewMatrix());

        object->Render();
        object2->Render();
        
        utility::DebugCameraMovement(window, camera);
        utility::DebugCameraMovementJoystick(window, camera);

        glfwSwapBuffers(window);
    }

    shader->End();
    glfwTerminate();
    return 0;
}