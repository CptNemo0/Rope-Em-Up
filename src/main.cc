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
#include "headers/TestAABB.h"
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
    const std::string kPlayerMeshPath = "res/models/player.obj";
    const std::string kDebugMeshPath = "res/models/debug_thingy.obj";

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
    auto player_mesh = std::make_shared<Mesh>(kPlayerMeshPath);
    auto debug_mesh = std::make_shared<Mesh>(kDebugMeshPath);

    auto object = std::make_shared<GameObject>(cube_mesh, green_texture, shader);
    auto object2 = std::make_shared<GameObject>(cube_mesh, red_texture, shader);
    auto debug_obj_1 = std::make_shared<GameObject>(debug_mesh, green_texture, shader);

    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.2f, 0.2f, 0.2f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    
    object->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    object2->transform_->set_position(glm::vec3(1.41421f * 2.0f, 0.0f, 1.42421f * 2.0f));
    
    
    auto collider1 = testaabb::CreateCollider(cube_mesh, object);
    auto collider2 = testaabb::CreateCollider(cube_mesh, object2);

    for (int i = 0; i < testaabb::colliders.size(); i++)
    {
        for (int j = i + 1; j < testaabb::colliders.size(); j++)
        {
            auto a = testaabb::colliders[i];
            auto b = testaabb::colliders[j];
            auto c = testaabb::TestAABBAABB(a, b);
        }
    }

    int a = 10;

    /*while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.03f, 0.04f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float previous_time = glfwGetTime();
        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        utility::DebugCameraMovement(window, camera, delta_time);
        utility::DebugCameraMovementJoystick(window, camera, delta_time);

        shader->Use();

        shader->SetVec3("camera_position", camera->get_position());
        shader->SetFloat("shininess", 50.0f);

        shader->SetPointLight("light", point_light);
        shader->SetMatrix4("projection_matrix", projection_matrix);
        shader->SetMatrix4("view_matrix", camera->GetViewMatrix());

        object->Render();
        object2->Render();
        debug_obj_1->Render();

        glfwSwapBuffers(window);
    }*/

    shader->End();
    glfwTerminate();
    return 0;
}