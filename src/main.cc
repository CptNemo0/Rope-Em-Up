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
#include "headers/Model.h"
#include "headers/MeshRenderer.h"
#include "headers/Physics.h"
#include "headers/Rope.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"
#include "headers/InputManager.h"
#include "headers/HUDRenderer.h"
#include "headers/TextRenderer.h"
#include "headers/HDRCubemap.h"

//unsigned int loadTexture(char const* path);
void renderSphere();

int main()
{
    const std::string kWindowTitle = "Modul Sumatywny";

    const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
    const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";

    const std::string kPBRVertexShaderPath = "res/shaders/PBRVertexShader.vert";
    const std::string kPBRFragmentShaderPath = "res/shaders/PBRFragmentShader.frag";
    
    const std::string kHDRCubemapVertexShaderPath = "res/shaders/HDRCubemapVertexShader.vert";
    const std::string kHDREquirectangularToCubemapFragmentShaderPath = "res/shaders/HDREquirectangularToCubemapFragmentShader.frag";
    const std::string kIrradianceFragmentShaderPath = "res/shaders/IrradianceConvolutionFragmentShader.frag";
    
    const std::string kBackgroundVertexShaderPath = "res/shaders/BackgroundVertexShader.vert";
    const std::string kBackgroundFragmentShaderPath = "res/shaders/BackgroundFragmentShader.frag";

    const std::string kHUDVertexShaderPath = "res/shaders/HUDVertexShader.vert";
    const std::string kHUDFragmentShaderPath = "res/shaders/HUDFragmentShader.frag";

    const std::string kHUDTextVertexShaderPath = "res/shaders/HUDTextVertexShader.vert";
    const std::string kHUDTextFragmentShaderPath = "res/shaders/HUDTextFragmentShader.frag";

    const std::string kGreenTexturePath = "res/textures/green_texture.png";
    const std::string kRedTexturePath = "res/textures/red_texture.png";
    const std::string kHUDTexturePath = "res/textures/placeholder_icon.png";
    const std::string kHUDTexturePath2 = "res/textures/staly_elmnt.png";

    const std::string kHDREquirectangularPath = "res/cubemaps/HDR_placeholder.hdr";

    const std::string kCubeMeshPath = "res/models/cube_2.obj";
    const std::string kPlayerMeshPath = "res/models/player.obj";
    const std::string kDebugMeshPath = "res/models/debug_thingy.obj";
    const std::string kEnemyMeshPath = "res/models/enemy.obj";
    const std::string kTestPath = "res/models/test2.obj";

    /*unsigned int albedo = loadTexture("res/textures/test/test_basecolor.png");
    unsigned int normal = loadTexture("res/textures/test/test_normal.png");
    unsigned int metallic = loadTexture("res/textures/test/test_metallic.png");
    unsigned int roughness = loadTexture("res/textures/test/test_roughness.png");*/
    //unsigned int ao = TextureFromFile("res/textures/test/ao.png", "");

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
    physics::PhysicsManager::Initialize();

    auto camera = std::make_shared<llr::Camera>();
    camera->set_fov(kFov);
    camera->set_near(kNear);
    camera->set_far(kFar);
    camera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());

    auto shader = std::make_shared<Shader>(kVertexShaderPath, kFragmentShaderPath);
    auto PBRShader = std::make_shared<Shader>(kPBRVertexShaderPath, kPBRFragmentShaderPath);
    auto EquirectangularToCubemapShader = std::make_shared<Shader>(kHDRCubemapVertexShaderPath, kHDREquirectangularToCubemapFragmentShaderPath);
    auto BackgroundShader = std::make_shared<Shader>(kBackgroundVertexShaderPath, kBackgroundFragmentShaderPath);
    auto IrradianceShader = std::make_shared<Shader>(kHDRCubemapVertexShaderPath, kIrradianceFragmentShaderPath);
    auto HUDshader = std::make_shared<Shader>(kHUDVertexShaderPath, kHUDFragmentShaderPath);
    auto HUDTextShader = std::make_shared<Shader>(kHUDTextVertexShaderPath, kHUDTextFragmentShaderPath);

    auto cubemap = std::make_shared<HDRCubemap>(kHDREquirectangularPath, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader);

    PointLight point_light;
    point_light.intensity = 100.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.ambient_colour = glm::vec3(0.6f, 0.6f, 0.6f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    // lights
    // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    auto test_model = std::make_shared<Model>(kTestPath);

    auto cube_model = std::make_shared<Model>(kCubeMeshPath);
    auto player_model = std::make_shared<Model>(kPlayerMeshPath);
    auto debug_model = std::make_shared<Model>(kDebugMeshPath);
    auto enemy_model = std::make_shared<Model>(kEnemyMeshPath);
    
    auto HUD_texture = std::make_shared<tmp::Texture>(kHUDTexturePath);
    auto HUD_texture2 = std::make_shared<tmp::Texture>(kHUDTexturePath2);

    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 1);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 2);
    //collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(1, 2);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(2, 2);

    auto scene_root = GameObject::Create();

    rope::Rope rope = rope::Rope();



    ////test
    auto test = GameObject::Create(scene_root);
    test->transform_->set_position(glm::vec3(-3.0f, 0.0f,-3.0f));
    test->AddComponent(std::make_shared<Components::MeshRenderer>(test_model, PBRShader));


    auto player_1 = GameObject::Create(scene_root);
    player_1->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    player_1->AddComponent(std::make_shared<Components::MeshRenderer>(player_model, shader));
    player_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_1->transform_));
    player_1->AddComponent(physics::PhysicsManager::i_->CreateParticle(player_1->transform_, 2.0f));
    player_1->AddComponent(std::make_shared<Components::RopeSegment>(nullptr, nullptr, player_1->transform_));

    rope.AddSegment(player_1->GetComponent<Components::RopeSegment>());

    auto player_2 = GameObject::Create(scene_root);
    player_2->transform_->set_position(glm::vec3(11.0f, 0.0f, 0.0f));
    player_2->AddComponent(std::make_shared<Components::MeshRenderer>(player_model, shader));
    player_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_2->transform_));
    player_2->AddComponent(physics::PhysicsManager::i_->CreateParticle(player_2->transform_, 2.0f));
    player_2->AddComponent(std::make_shared<Components::RopeSegment>(nullptr, nullptr, player_2->transform_));


    for (int i = 1; i < 20; i++)
    {
        auto new_object = GameObject::Create(scene_root);
        new_object->transform_->set_position(glm::vec3(i * rope::kMaxDistance, 0.0f, 0.0f));
        new_object->transform_->set_scale(glm::vec3(0.1f, 0.1f, 0.1f));
        new_object->AddComponent(std::make_shared<Components::MeshRenderer>(debug_model, shader));
        new_object->AddComponent(collisions::CollisionManager::i_->CreateCollider(2, gPRECISION, debug_model->meshes_[0], new_object->transform_));
        new_object->AddComponent(physics::PhysicsManager::i_->CreateParticle(new_object->transform_, 1.0f));
        new_object->AddComponent(std::make_shared<Components::RopeSegment>(nullptr, nullptr, new_object->transform_));
        rope.AddSegment(new_object->GetComponent<Components::RopeSegment>());
    }

    rope.AddSegment(player_2->GetComponent<Components::RopeSegment>());
    
    std::cout << rope.Size() << std::endl;


    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            auto new_object = GameObject::Create(scene_root);
            new_object->transform_->set_position(glm::vec3(i * 1, 0, j * 1));
            new_object->AddComponent(std::make_shared<Components::MeshRenderer>(player_model, shader));
            new_object->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], new_object->transform_));
            new_object->AddComponent(physics::PhysicsManager::i_->CreateParticle(new_object->transform_, 2.0f));
        }
    }

    auto HUD_root = GameObject::Create();

    auto HUD_object = GameObject::Create(HUD_root);
    HUD_object->AddComponent(std::make_shared<Components::HUDRenderer>(HUD_texture, HUDshader));
    HUD_object->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object->transform_->set_position(glm::vec3(-0.75f, -0.75f, 0.0f));

    auto HUD_object2 = GameObject::Create(HUD_root);
    HUD_object2->AddComponent(std::make_shared<Components::HUDRenderer>(HUD_texture2, HUDshader));
    HUD_object2->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object2->transform_->set_position(glm::vec3(0.75f, -0.75f, 0.0f));

    auto HUDText_root = GameObject::Create();

    auto HUDText_object = GameObject::Create(HUDText_root);
    HUDText_object->AddComponent(std::make_shared<Components::TextRenderer>(HUDTextShader, "..."));
    HUDText_object->transform_->set_scale(glm::vec3(0.005f, 0.005f, 1.0f));
    HUDText_object->transform_->set_position(glm::vec3(-0.95f, 0.95f, 0.0f));

    //----------------
    auto generator = std::make_shared<physics::BasicGenerator>();
    physics::PhysicsManager::i_->AddFGRRecord(generator, player_1->GetComponent<Components::Particle>());
    
    std::vector<physics::Contact> contacts = std::vector<physics::Contact>();

    //----------------sobie pogram w 

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    PBRShader->Use();
    /*PBRShader->SetInt("irradiance_map", 0);
    PBRShader->SetInt("albedo_map", 1);
    PBRShader->SetInt("normal_map", 2);
    PBRShader->SetInt("metallic_map", 3);
    PBRShader->SetInt("roughness_map", 4);
    PBRShader->SetFloat("ao", 1.0f);
    PBRShader->SetInt("ao_map", 4);*/

    PBRShader->SetInt("irradiance_map", 0);
    glm::vec3 albedo = glm::vec3(0.5f, 0.0f, 0.0f);
    PBRShader->SetVec3("albedo", albedo);
    PBRShader->SetFloat("ao", 1.0f);

    BackgroundShader->Use();
    BackgroundShader->SetInt("environmentMap", 0);

    cubemap->LoadHDRimg(window, camera);

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());
    PBRShader->Use();
    PBRShader->SetMatrix4("projection_matrix", projection);

    BackgroundShader->Use();
    BackgroundShader->SetMatrix4("projection_matrix", projection);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float previous_time = glfwGetTime();
        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        
#pragma region Collisions and Physics

        static float cp_time = 0;
        static int cp_idx = 0;

        std::chrono::steady_clock::time_point cp_begin = std::chrono::steady_clock::now();

        physics::PhysicsManager::i_->GeneratorUpdate();
        physics::PhysicsManager::i_->ParticleUpdate(delta_time);
        collisions::CollisionManager::i_->UpdateColliders();
        collisions::CollisionManager::i_->CollisionCheck(contacts);
        physics::PhysicsManager::i_->ResolveContacts(contacts);
        rope.EnforceRestraints(delta_time);
        std::chrono::steady_clock::time_point cp_end = std::chrono::steady_clock::now();

        cp_time += std::chrono::duration_cast<std::chrono::microseconds> (cp_end - cp_begin).count();
        cp_idx++;

        if (cp_idx == 120)
        {
            std::cout << "Collisions and Physic time = " << cp_time / cp_idx << "[micro s]" << std::endl;
            cp_idx = 0;
            cp_time = 0.0f;
        }

        

#pragma endregion
#pragma region Other

        utility::DebugCameraMovement(window, camera, delta_time);
        utility::DebugCameraMovementJoystick(window, camera, delta_time);

        shader->Use();

        shader->SetVec3("camera_position", camera->get_position());
        shader->SetFloat("shininess", 50.0f);

        shader->SetPointLight("light", point_light);
        shader->SetMatrix4("projection_matrix", projection_matrix);
        shader->SetMatrix4("view_matrix", camera->GetViewMatrix());


        PBRShader->Use();
        PBRShader->SetMatrix4("view_matrix", camera->GetViewMatrix());
        PBRShader->SetVec3("camera_position", camera->get_position());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->irradianceMap);

        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, albedo);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metallic);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, roughness);*/

        scene_root->PropagateUpdate();

        // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        glm::mat4 model = glm::mat4(1.0f);
        for (int row = 0; row < nrRows; ++row)
        {
            PBRShader->SetFloat("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                PBRShader->SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    (float)(col - (nrColumns / 2)) * spacing,
                    (float)(row - (nrRows / 2)) * spacing,
                    -2.0f
                ));
                PBRShader->SetMatrix4("model", model);
                PBRShader->SetMatrix3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
                renderSphere();
            }
        }


        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            PBRShader->SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            PBRShader->SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            PBRShader->SetMatrix4("model", model);
            PBRShader->SetMatrix3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();
        }

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        HUDshader->Use();

        HUD_root->PropagateUpdate();
        HUD_object->transform_->add_rotation(glm::vec3(133.0f * delta_time, 100.0f * delta_time, 66.0f * delta_time));

        HUDTextShader->Use();

        HUDText_root->PropagateUpdate();

        HUDText_object->GetComponent<Components::TextRenderer>()->ChangeText("fps: " + std::to_string(1.0f / delta_time));
        HUDText_root->PropagateUpdate();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        generator->direction_ = glm::vec3(0.0f);
        generator->magnitude_ = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_L))
        {
            generator->direction_ += glm::vec3(1.0f, 0.0f, 0.0f);
            generator->magnitude_ = 200;
        }

        if (glfwGetKey(window, GLFW_KEY_J))
        {
            generator->direction_ += glm::vec3(-1.0f, 0.0f, 0.0f);
            generator->magnitude_ = 200;
        }

        if (glfwGetKey(window, GLFW_KEY_I))
        {
            generator->direction_ += glm::vec3(0.0f, 0.0f, -1.0f);
            generator->magnitude_ = 200;
        }

        if (glfwGetKey(window, GLFW_KEY_K))
        {
            generator->direction_ += glm::vec3(0.0f, 0.0f, 1.0f);
            generator->magnitude_ = 200;
        }

        if (!(glfwGetKey(window, GLFW_KEY_L) || glfwGetKey(window, GLFW_KEY_I) || glfwGetKey(window, GLFW_KEY_K) || glfwGetKey(window, GLFW_KEY_J)))
        {
            generator->direction_ = glm::vec3(0.0f);
            generator->magnitude_ = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_O))
        {
            auto md = glm::vec3(0.0f, 10.0f, 0.0f);
            player_2->transform_->set_rotation(player_2->transform_->get_rotation() + md * delta_time);
        }

        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", camera->GetViewMatrix());

        cubemap->RenderCube();

        glfwSwapBuffers(window);

#pragma endregion
    }

    physics::PhysicsManager::Destroy();
    collisions::CollisionManager::Destroy();
    Input::InputManager::Destroy();
    
    shader->End();
    BackgroundShader->End();
    IrradianceShader->End();
    glfwTerminate();
    return 0;
}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}