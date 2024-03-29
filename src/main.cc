#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "headers/AABB.h"
#include "headers/Camera.h"
#include "headers/ConvexHull.h"
#include "headers/ConvexHullCreator.h"
#include "headers/GameObject.h"
#include "headers/Mesh.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"
#include "headers/MeshRenderer.h"

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
    const std::string kEnemyMeshPath = "res/models/enemy.obj";

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
    auto enemy_mesh = std::make_shared<Mesh>(kEnemyMeshPath);
    
    auto object = std::make_shared<GameObject>();
    object->AddComponent(std::make_shared<Components::MeshRenderer>(object->transform_, debug_mesh, green_texture, shader));

    auto object2 = std::make_shared<GameObject>();
    object2->AddComponent(std::make_shared<Components::MeshRenderer>(object2->transform_, enemy_mesh, red_texture, shader));

    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.2f, 0.2f, 0.2f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    object->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    object2->transform_->set_position(glm::vec3(0.5f, 0.0f, 0.5f));
    object2->transform_->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    auto aabb1 = collisions::CreateAABB(debug_mesh, object);
    auto aabb2 = collisions::CreateAABB(enemy_mesh, object2);

    auto chc = collisions::ConvexHullCreator(18);

    auto collider1 = chc.CreateConvexHull(debug_mesh);
    collider1->UpdateVertices(object->transform_->get_model_matrix());

    auto collider2 = chc.CreateConvexHull(enemy_mesh);
    collider2->UpdateVertices(object2->transform_->get_model_matrix());

    auto minkowski = collisions::MinkowskisDifference(collider1, collider2);
    
    

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
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

        object->Update();
        object2->Update();
        
        for (int i = 0; i < collisions::colliders.size(); i++)
        {
            for (int j = i + 1; j < collisions::colliders.size(); j++)
            {
                auto a = collisions::colliders[i];
                auto b = collisions::colliders[j];
                auto c = collisions::TestAABBAABB(a, b);
                if (c)
                {
                    std::cout << "Kolizja AABB\n";
                    auto polygon = collisions::MinkowskisDifference(collider1, collider2);
                    if (collisions::InsideDifference(polygon))
                    {
                        std::cout << "Kolizja minkowski\n";
                    }
                    else
                    {
                        std::cout << "Brak minkowski!\n";
                    }
                    if (glfwGetKey(window, GLFW_KEY_P))
                    {
                        collisions::WriteDebugFIles(polygon, collider1, collider2);
                    }
                }
                else
                {
                    std::cout<< "Brak kolizji AABB\n";
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_L))
        {
            auto md = glm::vec3(1.0f, 0.0f, 0.0f);
            object2->transform_->translate(md * delta_time);
            collider2->UpdateVertices(object2->transform_->get_model_matrix());
        }

        if (glfwGetKey(window, GLFW_KEY_J))
        {
            auto md = glm::vec3(-1.0f, 0.0f, 0.0f);
            object2->transform_->translate(md * delta_time);
            collider2->UpdateVertices(object2->transform_->get_model_matrix());
        }

        if (glfwGetKey(window, GLFW_KEY_I))
        {
            auto md = glm::vec3(0.0f, 0.0f, -1.0f);
            object2->transform_->translate(md * delta_time);
            collider2->UpdateVertices(object2->transform_->get_model_matrix());
        }

        if (glfwGetKey(window, GLFW_KEY_K))
        {
            auto md = glm::vec3(0.0f, 0.0f, +1.0f);
            object2->transform_->translate(md * delta_time);
            collider2->UpdateVertices(object2->transform_->get_model_matrix());
        }

        if (glfwGetKey(window, GLFW_KEY_O))
        {
            auto md = glm::vec3(0.0f, 10.0f, 0.0f);
            object2->transform_->set_rotation(object2->transform_->get_rotation() + md * delta_time);
            collider2->UpdateVertices(object2->transform_->get_model_matrix());
        }

        

        glfwSwapBuffers(window);
    }

    shader->End();
    glfwTerminate();
    return 0;
}