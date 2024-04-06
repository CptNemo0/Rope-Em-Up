#define STB_IMAGE_IMPLEMENTATION

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <ratio>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "headers/Camera.h"
#include "headers/Collider.h"
#include "headers/Collisions.h"
#include "headers/CollisionManager.h"
#include "headers/GameObject.h"
#include "headers/Model.h"
#include "headers/MeshRenderer.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"
#include "headers/InputManager.h"


int main()
{
    const std::string kWindowTitle = "Modul Sumatywny";

    const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
    const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";

    const std::string kGreenTexturePath = "res/textures/green_texture.png";
    const std::string kRedTexturePath = "res/textures/red_texture.png";

    const std::string kCubeMeshPath = "res/models/cube_2.obj";
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

    Input::InputManager::Initialize(window);
    collisions::CollisionManager::Initialize();

    auto camera = std::make_shared<llr::Camera>();
    camera->set_fov(kFov);
    camera->set_near(kNear);
    camera->set_far(kFar);
    camera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());

    auto shader = std::make_shared<Shader>(kVertexShaderPath, kFragmentShaderPath);

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.6f, 0.6f, 0.6f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    //auto green_texture = std::make_shared<Texture>(kGreenTexturePath);
    //auto red_texture = std::make_shared<Texture>(kRedTexturePath);

    //auto cube_model = std::make_shared<Model>(kCubeMeshPath);
    auto player_model = std::make_shared<Model>(kPlayerMeshPath);
    auto debug_model = std::make_shared<Model>(kDebugMeshPath);
    auto enemy_model = std::make_shared<Model>(kEnemyMeshPath);
    
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 1);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 2);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(1, 2);

    auto object = std::make_shared<GameObject>();
    object->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    object->AddComponent(std::make_shared<Components::MeshRenderer>(object->transform_, enemy_model, shader));
    object->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, enemy_model->meshes_, object->transform_));

    auto object2 = std::make_shared<GameObject>();
    object2->transform_->set_position(glm::vec3(0.5f, 0.0f, 0.5f));
    object2->AddComponent(std::make_shared<Components::MeshRenderer>(object2->transform_, debug_model, shader));
    object2->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, debug_model->meshes_, object2->transform_));

    std::vector<std::shared_ptr<GameObject>> gos;

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            auto new_object = std::make_shared<GameObject>();
            new_object->transform_->set_position(glm::vec3(i * 1, 0, j * 1));
            new_object->AddComponent(std::make_shared<Components::MeshRenderer>(new_object->transform_, player_model, shader));
            new_object->AddComponent(collisions::CollisionManager::i_->CreateCollider(2, gPRECISION, player_model->meshes_, new_object->transform_));
            
            gos.push_back(new_object);
        }
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float previous_time = glfwGetTime();
        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        collisions::CollisionManager::i_->CollisionCheck();

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
        
        for (auto& go : gos)
        {
            go->Update();
        }

        if (glfwGetKey(window, GLFW_KEY_L))
        {
            auto md = glm::vec3(2.0f, 0.0f, 0.0f);
            object2->transform_->translate(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_J))
        {
            auto md = glm::vec3(-2.0f, 0.0f, 0.0f);
           object2->transform_->translate(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_I))
        {
            auto md = glm::vec3(0.0f, 0.0f, -2.0f);
            object2->transform_->translate(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_K))
        {
            auto md = glm::vec3(0.0f, 0.0f, +2.0f);
            object2->transform_->translate(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_O))
        {
            auto md = glm::vec3(0.0f, 10.0f, 0.0f);
            object2->transform_->set_rotation(object2->transform_->get_rotation() + md * delta_time);
        }

        glfwSwapBuffers(window);
    }

    collisions::CollisionManager::Destroy();
    Input::InputManager::Destroy();
    
    shader->End();
    glfwTerminate();
    return 0;
}
