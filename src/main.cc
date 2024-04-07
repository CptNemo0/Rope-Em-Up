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
#include "stb_easy_font.h"

#include "headers/Camera.h"
#include "headers/Collider.h"
#include "headers/Collisions.h"
#include "headers/CollisionManager.h"
#include "headers/GameObject.h"
#include "headers/Mesh.h"
#include "headers/MeshRenderer.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"
#include "headers/InputManager.h"
#include "headers/HUDRenderer.h"

int main()
{
    const std::string kWindowTitle = "Modul Sumatywny";

    const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
    const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";
    const std::string kHUDVertexShaderPath = "res/shaders/HUDVertexShader.vert";
    const std::string kHUDFragmentShaderPath = "res/shaders/HUDFragmentShader.frag";

    const std::string kGreenTexturePath = "res/textures/green_texture.png";
    const std::string kRedTexturePath = "res/textures/red_texture.png";
    const std::string kHUDTexturePath = "res/textures/placeholder_icon.png";

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
    auto HUDshader = std::make_shared<Shader>(kHUDVertexShaderPath, kHUDFragmentShaderPath);

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.6f, 0.6f, 0.6f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    auto green_texture = std::make_shared<Texture>(kGreenTexturePath);
    auto red_texture = std::make_shared<Texture>(kRedTexturePath);
    auto HUD_texture = std::make_shared<Texture>(kHUDTexturePath, true);

    auto cube_mesh = std::make_shared<Mesh>(kCubeMeshPath);
    auto player_mesh = std::make_shared<Mesh>(kPlayerMeshPath);
    auto debug_mesh = std::make_shared<Mesh>(kDebugMeshPath);
    auto enemy_mesh = std::make_shared<Mesh>(kEnemyMeshPath);
    
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 1);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 2);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(1, 2);

    auto scene_root = GameObject::Create();

    auto object = GameObject::Create(scene_root);
    object->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    object->AddComponent(std::make_shared<Components::MeshRenderer>(enemy_mesh, green_texture, shader));
    object->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, enemy_mesh, object->transform_));

    auto object2 = GameObject::Create(scene_root);
    object2->transform_->set_position(glm::vec3(0.5f, 0.0f, 0.5f));
    object2->AddComponent(std::make_shared<Components::MeshRenderer>(debug_mesh, red_texture, shader));
    object2->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, debug_mesh, object2->transform_));

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            auto new_object = GameObject::Create(scene_root);
            new_object->transform_->set_position(glm::vec3(i * 1, 0, j * 1));
            new_object->AddComponent(std::make_shared<Components::MeshRenderer>(player_mesh, green_texture, shader));
            new_object->AddComponent(collisions::CollisionManager::i_->CreateCollider(2, gPRECISION, player_mesh, new_object->transform_));
        }
    }

    auto HUD_root = GameObject::Create();

    auto HUD_object = GameObject::Create(HUD_root);
    HUD_object->AddComponent(std::make_shared<HUDRenderer>(HUD_texture, HUDshader));
    HUD_object->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object->transform_->set_position(glm::vec3(-0.75f, -0.75f, 0.0f));

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

        scene_root->PropagateUpdate();

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        HUDshader->Use();

        HUD_root->PropagateUpdate();
        HUD_object->transform_->add_rotation(glm::vec3(133.0f * delta_time, 100.0f * delta_time, 66.0f * delta_time));

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        if (glfwGetKey(window, GLFW_KEY_L))
        {
            auto md = glm::vec3(2.0f, 0.0f, 0.0f);
            object2->transform_->add_position(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_J))
        {
            auto md = glm::vec3(-2.0f, 0.0f, 0.0f);
            object2->transform_->add_position(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_I))
        {
            auto md = glm::vec3(0.0f, 0.0f, -2.0f);
            object2->transform_->add_position(md * delta_time);
        }

        if (glfwGetKey(window, GLFW_KEY_K))
        {
            auto md = glm::vec3(0.0f, 0.0f, +2.0f);
            object2->transform_->add_position(md * delta_time);
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
