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

void renderCube();

int main()
{
    const std::string kWindowTitle = "Modul Sumatywny";

    const std::string kVertexShaderPath = "res/shaders/BasicVertexShader.vert";
    const std::string kFragmentShaderPath = "res/shaders/BasicFragmentShader.frag";

    const std::string kPBRVertexShaderPath = "res/shaders/PBRVertexShader.vert";
    const std::string kPBRFragmentShaderPath = "res/shaders/PBRFragmentShader.frag";
    
    const std::string kHDRCubemapVertexShaderPath = "res/shaders/HDRCubemapVertexShader.vert";
    const std::string kHDREquirectangularToCubemapFragmentShaderPath = "res/shaders/HDREquirectangularToCubemapFragmentShader.frag";
    
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
    auto HUDshader = std::make_shared<Shader>(kHUDVertexShaderPath, kHUDFragmentShaderPath);
    auto HUDTextShader = std::make_shared<Shader>(kHUDTextVertexShaderPath, kHUDTextFragmentShaderPath);

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


    //auto green_texture = std::make_shared<Texture>(kGreenTexturePath);
    //auto red_texture = std::make_shared<Texture>(kRedTexturePath);

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

    //----------------

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    /*PBRShader->Use();
    glm::vec3 albedo = glm::vec3(0.5f, 0.0f, 0.0f);
    PBRShader->SetVec3("albedo_map", albedo);
    PBRShader->SetFloat("ao_map", 1.0f);*/

    BackgroundShader->Use();
    BackgroundShader->SetInt("environmentMap", 0);

    // pbr: setup framebuffer
    // ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // pbr: load the HDR environment map
    // ---------------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf(kHDREquirectangularPath.c_str(), &width, &height, &nrComponents, 0);
    unsigned int hdrTexture = 0;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }

    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    EquirectangularToCubemapShader->Use();
    EquirectangularToCubemapShader->SetInt("equirectangularMap", 0);
    EquirectangularToCubemapShader->SetMatrix4("projection_matrix", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        EquirectangularToCubemapShader->SetMatrix4("view_matrix", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());
    /*PBRShader->Use();
    PBRShader->SetMatrix4("projection", projection);*/
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

       /* PBRShader->Use();
        PBRShader->SetMatrix4("view_matrix", camera->GetViewMatrix());
		PBRShader->SetVec3("camera_position", camera->get_position());*/


        scene_root->PropagateUpdate();

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

        // render skybox (render as last to prevent overdraw)
        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", camera->GetViewMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        renderCube();

        glfwSwapBuffers(window);

#pragma endregion
    }

    physics::PhysicsManager::Destroy();
    collisions::CollisionManager::Destroy();
    Input::InputManager::Destroy();
    
    shader->End();
    glfwTerminate();
    return 0;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}