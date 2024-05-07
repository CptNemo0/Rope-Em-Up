#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <ratio>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "stb_easy_font.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "headers/global.h"

#include "headers/Camera.h"
#include "headers/collisions/Collider.h"
#include "headers/collisions/Collisions.h"
#include "headers/collisions/CollisionManager.h"
#include "headers/collisions/Raycast.h"
#include "headers/GameObject.h"
#include "headers/Model.h"
#include "headers/components/MeshRenderer.h"
#include "headers/physics/Physics.h"
#include "headers/physics/PBD.h"
#include "headers/physics/Rope.h"
#include "headers/Postprocessing.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/utility.h"
#include "headers/input/InputManager.h"
#include "headers/components/HUDRenderer.h"
#include "headers/components/TextRenderer.h"
#include "headers/components/PlayerController.h"
#include "headers/HDRCubemap.h"
#include "headers/Font.h"
#include "headers/components/ParticleEmitter.h"
#include "headers/ParticleEmitterManager.h"
#include "headers/generation/RoomGenerator.h"
#include "headers/audio/AudioManager.h"
#include "headers/audio/Sounds.h"
#include "headers/components/AudioSource.h"

#include "headers/SteeringBehaviors.h"
#include "headers/Vehicle.h"
#include "headers/ai/EnemyAIManager.h"
#include "headers/ai/EnemyState.h"
#include "headers/ai/EnemyStateMachine.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include "headers/LBuffer.h"
#include "headers/GBuffer.h"
#include "headers/SSAO.h"

void FixOrientation(s_ptr<GameObject> go)
{
    auto current_forward = go->transform_->get_position() - go->transform_->get_previous_position();
    if (glm::length(current_forward))
    {
        current_forward = glm::normalize(current_forward);
    }
    float angle = glm::degrees(glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), current_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    go->transform_->set_rotation(glm::vec3(0.0f, angle, 0.0f));
}

void BindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main()
{
    char zaza;
    cout << "Byc czy nie byc oto jest pytanie.\n";
    ///// PATH TO RESOURCES
#pragma region Res Path
    const string kWindowTitle = "Rope'em Up!";

    const string kVertexShaderPath = "res/shaders/Basic.vert";
    const string kFragmentShaderPath = "res/shaders/Basic.frag";

    const string kHUDVertexShaderPath = "res/shaders/HUD.vert";
    const string kHUDFragmentShaderPath = "res/shaders/HUD.frag";

    const string kHUDTextVertexShaderPath = "res/shaders/HUDText.vert";
    const string kHUDTextFragmentShaderPath = "res/shaders/HUDText.frag";

    const string kPBRVertexShaderPath = "res/shaders/PBR.vert";
    const string kPBRFragmentShaderPath = "res/shaders/PBR.frag";

    const string kHDRCubemapVertexShaderPath = "res/shaders/HDRCubemap.vert";
    const string kHDREquirectangularToCubemapFragmentShaderPath = "res/shaders/HDREquirectangularToCubemap.frag";
    const string kIrradianceFragmentShaderPath = "res/shaders/IrradianceConvolution.frag";

    const string kBackgroundVertexShaderPath = "res/shaders/Background.vert";
    const string kBackgroundFragmentShaderPath = "res/shaders/Background.frag";

    const string kParticleVertexShaderPath = "res/shaders/Particle.vert";
    const string kParticleGeometryShaderPath = "res/shaders/Particle.geom";
    const string kParticleFragmentShaderPath = "res/shaders/Particle.frag";

    const string kPostprocessingVertexShaderPath = "res/shaders/PostprocessingShader.vert";
    const string kPostprocessingFragmentShaderPath = "res/shaders/PostprocessingShader.frag";

    const string kGBufferVertexShaderPath = "res/shaders/GBufferPass.vert";
    const string kGBufferFragmentShaderPath = "res/shaders/GBufferPass.frag";

    const string kLBufferVertexShaderPath = "res/shaders/LBufferPass.vert";
    const string kLBufferFragmentShaderPath = "res/shaders/LBufferPass.frag";

    const string kSSAOVertexShaderPath = "res/shaders/SSAO.vert";
    const string kSSAOFragmentShaderPath = "res/shaders/SSAO.frag";

    const string kSSAOBlurVertexShaderPath = "res/shaders/SSAOBlur.vert";
    const string kSSAOBlurFragmentShaderPath = "res/shaders/SSAOBlur.frag";

    const string kGreenTexturePath = "res/textures/green_texture.png";
    const string kRedTexturePath = "res/textures/red_texture.png";
    const string kHUDTexturePath = "res/textures/placeholder_icon.png";
    const string kHUDTexturePath2 = "res/textures/staly_elmnt.png";
    const string kTestSmokeTexturePath = "res/textures/test_smoke.png";

    const string kHDREquirectangularPath = "res/cubemaps/HDR_placeholder.hdr";

    const string kCubeMeshPath = "res/models/cube_2.obj";
    const string kPlayerMeshPath = "res/models/player.obj";
    const string kDebugMeshPath = "res/models/debug_thingy.obj";
    const string kEnemyMeshPath = "res/models/enemy.obj";
    const string kTestPath = "res/models/test2.obj";
    const string kWallPath = "res/models/simple_wall.obj";
    const string kModule1Path = "res/models/module1.obj";
    const string kModule2Path = "res/models/enviroment/modules/module2.obj";
    const string kSimpleFloodPath = "res/models/enviroment/floor/floor.obj";
    const string kTestBallPath = "res/models/test_ball.obj";
    const string kGatePath = "res/models/gate.obj";

    const string kFontPath = "res/fonts/CourierPrime-Regular.ttf";

    const string kBruhPath = "res/sounds/bruh.wav";
#pragma endregion Resouces Path
    /////
    
    audio::AudioManager::Initialize();
    audio::AudioManager::i_->LoadSound(audio::Sounds::bruh, kBruhPath);

    ///// CAMERA SETTINGS
#pragma region CameraSettings
    const float kFov = 90.0f;
    const float kNear = 0.1f;
    const float kFar = 1000.0f;
    float kpitch = -90.0f;
    float kyaw = 90.0f;
    int chosen_camera = 0;
    std::vector<llr::Camera> cameras;

#pragma endregion CameraSettings


    srand(static_cast <unsigned> (time(0)));

    Vehicle enemy_vehicle_template;

    enemy_vehicle_template.rest_lenght = 4.0f;

    enemy_vehicle_template.max_speed = 2000.0f;

    enemy_vehicle_template.wander_target = glm::vec3(0.0f);
    enemy_vehicle_template.wander_distance = 2.0f;
    enemy_vehicle_template.wander_radius = 2.0f;
    enemy_vehicle_template.wander_jitter = 0.5f;
    enemy_vehicle_template.wander_weight = 1.0f;
    enemy_vehicle_template.wander_speed_ = 1000.f;

    enemy_vehicle_template.wall_avoidance_distance = 2.0f;
    enemy_vehicle_template.wall_avoidance_weight = 3.0f;

    enemy_vehicle_template.pursuit_distance = 0.5f;
    enemy_vehicle_template.pursuit_weight = 1.0f;
    enemy_vehicle_template.pursuit_speed_ = 1500.f;

    enemy_vehicle_template.extrapolation_distance = 5.0f;
    enemy_vehicle_template.extrapolation_weight = 1.0f;
    enemy_vehicle_template.extrapolation_speed_ = 1600.f;

    enemy_vehicle_template.evade_distance = 5.0f;
    enemy_vehicle_template.evade_weight = 1.0f;
    enemy_vehicle_template.evade_speed_ = 1700.f;

    ai::EnemyAIManagerInitStruct enemy_ai_init;
    enemy_ai_init.choked_tentacles = 10;
    enemy_ai_init.multi_chokes = 0;
    enemy_ai_init.choke_threshold = 5;
    enemy_ai_init.multi_threshold = 5;
    enemy_ai_init.wall_proximity_threshold = 1.0f;
    enemy_ai_init.attack_damage = 1.0f;
    enemy_ai_init.attack_range = 2.5f;
    enemy_ai_init.sense_range = 7.0f;

    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    GLFWvidmode* mode = nullptr;



    if (int return_value = utility::InitGLFW(window, monitor, mode, kWindowTitle))
    {
        exit(return_value);
    }
    cout << "GLFW Initialized.\n";
    

    if (int return_value = utility::InitGlad())
    {   
        exit(return_value);
    }
    cout << "GLAD Initialized.\n";
    
    utility::InitImGUI(window);

    input::InputManager::Initialize(window);
    collisions::CollisionManager::Initialize();
    physics::PhysicsManager::Initialize();
    pbd::PBDManager::Initialize(3, 0.5f, 0.8f);
    ai::EnemyAIManager::Initialize(enemy_ai_init, enemy_vehicle_template);
    ParticleEmitterManager::Initialize();

#pragma region CamerasConfiguration
    auto camera = make_shared<llr::Camera>();
    camera->set_fov(kFov);
    camera->set_near(kNear);
    camera->set_far(kFar);
    camera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    camera->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->set_pitch(-90.0f);
    camera->set_yaw(-90.0f);

    auto debugCamera = make_shared<llr::Camera>();
    debugCamera->set_fov(kFov);
    debugCamera->set_near(kNear);
    debugCamera->set_far(kFar);
    debugCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    debugCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    debugCamera->set_pitch(-90.0f);
    debugCamera->set_yaw(-90.0f);

    cameras.push_back(*camera);
    cameras.push_back(*debugCamera);
#pragma endregion CamerasConfiguration

    
    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());
    auto ortho_matrix = glm::ortho(0.0f, (float)mode->width, 0.0f, (float)mode->height);
    
    
    //auto shader = make_shared<Shader>(kVertexShaderPath, kFragmentShaderPath);
    auto HUDshader = make_shared<Shader>(kHUDVertexShaderPath, kHUDFragmentShaderPath);
    auto HUDTextShader = make_shared<Shader>(kHUDTextVertexShaderPath, kHUDTextFragmentShaderPath);
    auto PBRShader = make_shared<Shader>(kPBRVertexShaderPath, kPBRFragmentShaderPath);
    auto EquirectangularToCubemapShader = make_shared<Shader>(kHDRCubemapVertexShaderPath, kHDREquirectangularToCubemapFragmentShaderPath);
    auto BackgroundShader = make_shared<Shader>(kBackgroundVertexShaderPath, kBackgroundFragmentShaderPath);
    auto IrradianceShader = make_shared<Shader>(kHDRCubemapVertexShaderPath, kIrradianceFragmentShaderPath);
    auto ParticleShader = make_shared<Shader>(kParticleVertexShaderPath, kParticleGeometryShaderPath, kParticleFragmentShaderPath);
    auto PostprocessingShader = make_shared<Shader>(kPostprocessingVertexShaderPath, kPostprocessingFragmentShaderPath);
    auto GBufferPassShader = make_shared<Shader>(kGBufferVertexShaderPath, kGBufferFragmentShaderPath);
    auto LBufferPassShader = make_shared<Shader>(kLBufferVertexShaderPath, kLBufferFragmentShaderPath);
    auto SSAOShader = make_shared<Shader>(kSSAOVertexShaderPath, kSSAOFragmentShaderPath);
    auto SSAOBlurShader = make_shared<Shader>(kSSAOBlurVertexShaderPath, kSSAOBlurFragmentShaderPath);

    auto HUD_texture = make_shared<tmp::Texture>(kHUDTexturePath);
    auto HUD_texture2 = make_shared<tmp::Texture>(kHUDTexturePath2);
    auto Smoke_texture = make_shared<tmp::Texture>(kTestSmokeTexturePath);

    auto a = glm::mat3(1.0f) * glm::vec3(0.0f);

    LBuffer lbuffer = LBuffer(mode->height, mode->width);
    GBuffer gbuffer = GBuffer(mode->height, mode->width);
    SSAOBuffer ssao_buffer = SSAOBuffer(mode->height, mode->width, SSAOPrecision::MEDIUM_SSAO);
    SSAOBlurBuffer ssao_blur_buffer = SSAOBlurBuffer(mode->height, mode->width);
    ppc::Postprocessor postprocessor = ppc::Postprocessor(mode->width, mode->height, PostprocessingShader);

    auto cubemap = make_shared<HDRCubemap>(kHDREquirectangularPath, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader);

    PointLight point_light;
    point_light.intensity = 200.0f;
    point_light.position = glm::vec3(-3.0f, 3.0f, -3.0f);
    point_light.ambient_colour = glm::vec3(0.6f, 0.6f, 0.6f);
    point_light.diffuse_colour = glm::vec3(0.5f, 0.7f, 0.5f);
    point_light.specular_colour = glm::vec3(0.5f, 0.7f, 0.5f);

    // lights
    // ------
    glm::vec3 light_Positions[] = {
        glm::vec3(0.0f,  10.0f, 0.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 light_Colors[] = {
        glm::vec3(23.47, 21.31, 20.79),
        glm::vec3(23.47, 21.31, 20.79),
        glm::vec3(23.47, 21.31, 20.79),
        glm::vec3(23.47, 21.31, 20.79)
    };

    auto test_model = make_shared<Model>(kTestPath);

    auto cube_model = make_shared<Model>(kCubeMeshPath);
    auto player_model = make_shared<Model>(kPlayerMeshPath);
    auto debug_model = make_shared<Model>(kDebugMeshPath);
    auto enemy_model = make_shared<Model>(kEnemyMeshPath);
    auto wall_model = make_shared<Model>(kWallPath);
    auto module_1_model = make_shared<Model>(kModule1Path);
    auto module_2_model = make_shared<Model>(kModule2Path);
    auto simple_floor_model = make_shared<Model>(kSimpleFloodPath);
    auto test_ball_model = make_shared<Model>(kTestBallPath);
    auto gate_model = make_shared<Model>(kGatePath);

    generation::RoomModels models;
    models.walls.push_back(module_2_model);
    models.floors.push_back(simple_floor_model);
    models.gates.push_back(gate_model);

    generation::RoomGenerationSettings rg_settings;
    rg_settings.width = 5;
    rg_settings.height = 4;

    std::deque<w_ptr<GameObject>> room_parts;

    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 1);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 2);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(2, 2);

    auto scene_root = GameObject::Create();

    generation::GenerateRoom(&rg_settings, &models, room_parts, scene_root, GBufferPassShader);

    /*auto gate_1 = GameObject::Create(scene_root);
    gate_1->AddComponent(make_shared<components::MeshRenderer>(gate_model, GBufferPassShader));

    auto gate_2 = GameObject::Create(scene_root);
    gate_2->AddComponent(make_shared<components::MeshRenderer>(gate_model, GBufferPassShader));

    auto wall_up_1 = GameObject::Create(scene_root);
    wall_up_1->transform_->set_position(glm::vec3(-8.0f, 0.0f, 0.0f));
    wall_up_1->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
    wall_up_1->AddComponent(make_shared<components::MeshRenderer>(module_2_model, GBufferPassShader));
    wall_up_1->transform_->AddChild(gate_1->transform_);

    auto wall_up_2 = GameObject::Create(scene_root);
    wall_up_2->transform_->set_position(glm::vec3(-8.0f - generation::kModuleSize, 0.0f, 0.0f));
    wall_up_2->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
    wall_up_2->AddComponent(make_shared<components::MeshRenderer>(module_2_model, GBufferPassShader));
    
    auto wall_right_1 = GameObject::Create(scene_root);
    wall_right_1->transform_->set_position(glm::vec3(0.0f, 0.0f, -8.0f));
    wall_right_1->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
    wall_right_1->AddComponent(make_shared<components::MeshRenderer>(module_2_model, GBufferPassShader));
    wall_right_1->transform_->AddChild(gate_2->transform_);

    auto wall_right_2 = GameObject::Create(scene_root);
    wall_right_2->transform_->set_position(glm::vec3(0, 0.0f, -8.0f - generation::kModuleSize));
    wall_right_2->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
    wall_right_2->AddComponent(make_shared<components::MeshRenderer>(module_2_model, GBufferPassShader));

    auto floor_1 = GameObject::Create(scene_root);
    floor_1->transform_->set_position(glm::vec3(-8.0f, 0.0f, -8.0f));
    floor_1->transform_->set_scale(glm::vec3(1.0f, 0.0f, 1.0f));
    floor_1->AddComponent(make_shared<components::MeshRenderer>(simple_floor_model, GBufferPassShader));

    auto floor_2 = GameObject::Create(scene_root);
    floor_2->transform_->set_position(glm::vec3(-8.0f, 0.0f, -1.5f * generation::kModuleSize));
    floor_2->transform_->set_scale(glm::vec3(1.0f, 0.0f, 1.0f));
    floor_2->AddComponent(make_shared<components::MeshRenderer>(simple_floor_model, GBufferPassShader));

    auto floor_3 = GameObject::Create(scene_root);
    floor_3->transform_->set_position(glm::vec3(-1.5f * generation::kModuleSize, 0.0f, -1.5f * generation::kModuleSize));
    floor_3->transform_->set_scale(glm::vec3(1.0f, 0.0f, 1.0f));
    floor_3->AddComponent(make_shared<components::MeshRenderer>(simple_floor_model, GBufferPassShader));

    auto floor_4 = GameObject::Create(scene_root);
    floor_4->transform_->set_position(glm::vec3(-1.5f * generation::kModuleSize, 0.0f, -8.0f));
    floor_4->transform_->set_scale(glm::vec3(1.0f, 0.0f, 1.0f));
    floor_4->AddComponent(make_shared<components::MeshRenderer>(simple_floor_model, GBufferPassShader));
    */
    pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-2.0 * generation::kModuleSize, 0.0f, -2.0 * generation::kModuleSize), 1.0f);
    pbd::PBDManager::i_->set_walls(walls);

    auto enemy_1 = GameObject::Create(scene_root);
    enemy_1->transform_->set_position(glm::vec3(-10.0f, 0.0f, -10.0f));    
    enemy_1->transform_->set_position(glm::vec3(-10.0f, 0.0f, -10.0f));
    enemy_1->AddComponent(make_shared<components::MeshRenderer>(enemy_model, GBufferPassShader));
    enemy_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, enemy_model->meshes_[0], enemy_1->transform_));
    enemy_1->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy_1->transform_));
    auto enemy_movement_generator_1 = make_shared<pbd::BasicGenerator>();
    pbd::PBDManager::i_->CreateFGRRecord(enemy_1->GetComponent<components::PBDParticle>(), enemy_movement_generator_1);
    auto enemy_state_machine_1 = make_shared<ai::EnemyStateMachine>(enemy_1, enemy_movement_generator_1, enemy_vehicle_template);

    auto enemy_2 = GameObject::Create(scene_root);
    enemy_2->transform_->set_position(glm::vec3(-8.0f, 0.0f, -10.0f));
    enemy_2->transform_->set_position(glm::vec3(-8.0f, 0.0f, -10.0f));
    enemy_2->AddComponent(make_shared<components::MeshRenderer>(enemy_model, GBufferPassShader));
    enemy_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, enemy_model->meshes_[0], enemy_2->transform_));
    enemy_2->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy_2->transform_));
    auto enemy_movement_generator_2 = make_shared<pbd::BasicGenerator>();
    pbd::PBDManager::i_->CreateFGRRecord(enemy_2->GetComponent<components::PBDParticle>(), enemy_movement_generator_2);
    auto enemy_state_machine_2 = make_shared<ai::EnemyStateMachine>(enemy_2, enemy_movement_generator_2, enemy_vehicle_template);

    ////test
    /*auto test = GameObject::Create(scene_root);
    test->transform_->set_position(glm::vec3(-3.0f, 2.0f, -3.0f));
    test->AddComponent(make_shared<components::MeshRenderer>(test_model, PBRShader));*/

    auto player_1 = GameObject::Create(scene_root);
    player_1->transform_->set_position(glm::vec3(-0.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_1->transform_->set_position(glm::vec3(-0.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_1->AddComponent(make_shared<components::MeshRenderer>(player_model, GBufferPassShader));
    player_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_1->transform_));
    player_1->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_1->transform_));
    player_1->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_1));

    auto player_2 = GameObject::Create(scene_root);
    player_2->transform_->set_position(glm::vec3(-1.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_2->transform_->set_position(glm::vec3(-1.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_2->AddComponent(make_shared<components::MeshRenderer>(player_model, GBufferPassShader));
    player_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_2->transform_));
    player_2->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_2->transform_));
    player_2->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_2));

    //ai::EnemyAIManager::SetPlayers(player_1, player_2);
    ////ai::EnemyAIManager::SetEnemies(enemies) //jakis vector i potem metoda ktora go zmienia na cos innego moze zadziala
    auto gameplayCamera = GameObject::Create(scene_root);
    gameplayCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));

    gameplayCamera->AddComponent(make_shared<components::GameplayCameraComponent>(player_1, player_2, camera));
    auto gameplayCameraComponent = gameplayCamera->GetComponent<components::GameplayCameraComponent>();

    //std::vector<s_ptr<GameObject>> rope_segments;

    //for (int i = 0; i < 40; i++)
    //{
    //    auto rope_segment = GameObject::Create(scene_root);
    //    rope_segment->transform_->set_scale(glm::vec3(1.1f, 1.1f, 1.1f));
    //    rope_segment->transform_->set_position(glm::vec3(((float)i + 1.0f) / 5.0f, 0.0f, 0.0f));
    //    rope_segment->transform_->set_position(glm::vec3(((float)i + 1.0f) / 5.0f, 0.0f, 0.0f));
    //    rope_segment->AddComponent(make_shared<components::MeshRenderer>(test_ball_model, GBufferPassShader));
    //    rope_segment->AddComponent(collisions::CollisionManager::i_->CreateCollider(2, gPRECISION, test_ball_model->meshes_[0], rope_segment->transform_));
    //    rope_segment->AddComponent(pbd::PBDManager::i_->CreateParticle(0.25f, 0.99f, rope_segment->transform_));

    //    if (i == 0)
    //    {
    //        pbd::PBDManager::i_->CreateRopeConstraint(player_1->GetComponent<components::PBDParticle>(), rope_segment->GetComponent<components::PBDParticle>(), 0.31f);
    //    }
    //    else
    //    {
    //        pbd::PBDManager::i_->CreateRopeConstraint(rope_segments.back()->GetComponent<components::PBDParticle>(), rope_segment->GetComponent<components::PBDParticle>(), 0.31f);
    //    }

    //    rope_segments.push_back(rope_segment);
    //}

    //pbd::PBDManager::i_->CreateRopeConstraint(rope_segments.back()->GetComponent<components::PBDParticle>(), player_2->GetComponent<components::PBDParticle>(), 0.21f);


    auto HUD_root = GameObject::Create();

    auto HUD_object = GameObject::Create(HUD_root);
    HUD_object->AddComponent(make_shared<components::HUDRenderer>(HUD_texture, HUDshader));
    HUD_object->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object->transform_->set_position(glm::vec3(-0.75f, -0.75f, 0.0f));

    auto HUD_object2 = GameObject::Create(HUD_root);
    HUD_object2->AddComponent(make_shared<components::HUDRenderer>(HUD_texture2, HUDshader));
    HUD_object2->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object2->transform_->set_position(glm::vec3(0.75f, -0.75f, 0.0f));

    auto HUDText_root = GameObject::Create();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        return -1;
    }
    auto maturasc_font = make_shared<Font>(ft, kFontPath.c_str());

    auto HUDText_object = GameObject::Create(HUDText_root);
    HUDText_object->AddComponent(make_shared<components::TextRenderer>(HUDTextShader, maturasc_font, "TEST", glm::vec3(1.0f)));
    HUDText_object->transform_->set_scale(glm::vec3(1.0f, 1.0f, 1.0f));
    HUDText_object->transform_->set_position(glm::vec3(50.0f, 900.0f, 0.0f));

    auto particle_root = GameObject::Create();

    auto particle_emitter = GameObject::Create(player_1);
    particle_emitter->transform_->set_position(glm::vec3(0.0f, 0.5f, 0.0f));
    particle_emitter->AddComponent(make_shared<components::ParticleEmitter>(100, Smoke_texture, ParticleShader, gameplayCameraComponent->camera_));
    auto particle_emitter_component = particle_emitter->GetComponent<components::ParticleEmitter>();
    particle_emitter_component->emission_rate_ = 0.1f;
    particle_emitter_component->start_acceleration_ = glm::vec3(0.0f, 9.81f, 0.0f);
    particle_emitter_component->start_size_ = glm::vec2(0.1f, 0.0f);
    particle_emitter_component->end_size_ = glm::vec2(0.5f, 1.0f);
    particle_emitter_component->start_position_displacement_ = 1.0f;

    generation::RoomLayoutGenerationSettings rlgs;
    rlgs.angle = 0.5f;
    rlgs.span = 0.5f;
    rlgs.branch_division_count = 4;
    rlgs.branch_division_min_length = 2.0f;
    rlgs.branch_division_max_length = 3.0f;
    rlgs.sub_branch_count = 3;
    rlgs.sub_branch_span = 0.2f;
    rlgs.sub_branch_min_length = 3.0f;
    rlgs.sub_branch_max_length = 4.0f;

    generation::RoomLayoutGenerator rlg;
    std::deque<w_ptr<GameObject>> room_objects;

    for (auto &room : rlg.rooms)
    {
        cout << '(' << room.first.x << ", " << room.first.y << ')' << endl;
        auto room_obj = GameObject::Create(scene_root);
        room_objects.push_back(room_obj);
        room_obj->AddComponent(make_shared<components::MeshRenderer>(test_ball_model, GBufferPassShader));
        room_obj->transform_->set_position(glm::vec3(room.first.x, 6.0f, room.first.y));
        room_obj->transform_->set_scale(glm::vec3(3.0f));
    }

    auto audio_test_obj = GameObject::Create(scene_root);
    audio_test_obj->AddComponent(make_shared<components::AudioSource>());

    scene_root->PropagateStart();
    HUD_root->PropagateStart();
    HUDText_root->PropagateStart();
    particle_root->PropagateStart();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    PBRShader->Use();
    PBRShader->SetInt("irradiance_map", 5);

    BackgroundShader->Use();
    BackgroundShader->SetInt("environmentMap", 0);

    cubemap->LoadHDRimg(window, gameplayCameraComponent->camera_);

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(gameplayCameraComponent->camera_->get_fov()), gameplayCameraComponent->camera_->get_aspect_ratio(), gameplayCameraComponent->camera_->get_near(), gameplayCameraComponent->camera_->get_far());
    PBRShader->Use();
    PBRShader->SetMatrix4("projection_matrix", projection);

    ParticleShader->Use();
    ParticleShader->SetMatrix4("projection_matrix", projection);

    BackgroundShader->Use();
    BackgroundShader->SetMatrix4("projection_matrix", projection);

    SSAOShader->Use();
    SSAOShader->SetMatrix4("projection_matrix", projection);
    SSAOShader->SetInt("height", mode->height);
    SSAOShader->SetInt("width", mode->width);
    SSAOShader->SetInt("quality",(int)ssao_buffer.quality_);
    SSAOShader->SetFloat("radius", 0.5);
    SSAOShader->SetFloat("bias", 0.0025);
    ssao_buffer.SetKernel(SSAOShader);


    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    Timer::Timer fixed_update_timer = Timer::CreateTimer(1.0f / 120.0f, [enemy_state_machine_1, enemy_state_machine_2, &fixed_update_timer]()
    {
        //ai::EnemyAIManager::i_->UpdateEnemyStateMachine(enemy_state_machine_1);
        //ai::EnemyAIManager::i_->UpdateEnemyStateMachine(enemy_state_machine_2);

        pbd::PBDManager::i_->GeneratorUpdate();
        pbd::PBDManager::i_->Integration(pbd::kMsPerUpdate);
        collisions::CollisionManager::i_->PredictColliders();
        collisions::CollisionManager::i_->CollisionCheckPBD(pbd::PBDManager::i_->contacts_);
        pbd::PBDManager::i_->ProjectConstraints(pbd::kMsPerUpdate);
        pbd::PBDManager::i_->UpdatePositions(pbd::kMsPerUpdate);
        pbd::PBDManager::i_->ClearContacts();
        
        ParticleEmitterManager::i_->Update(pbd::kMsPerUpdate);

    }, nullptr, true);

    // wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //////////////////////////////////////////
    //////////////////////////////////////////
    //////////////////////////////////////////
    // GAME LOOP GAME LOOP GAME LOOP GAME LOOP 
    //////////////////////////////////////////
    //////////////////////////////////////////
    //////////////////////////////////////////

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float gamma_value = 1.0f;
        static float brightness_value = 0.0f;
        static float contrast_value = 1.0f;

        static float lag = 0.0f;
        static float previous_time = glfwGetTime();
        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        lag += delta_time;
        #ifdef _DEBUG
            delta_time = glm::clamp(delta_time, 0.0f, (1.0f / 30.0f));
        #endif

        previous_time = current_time;

        //physics::LogVec3(player_1->transform_->get_position());
    
        Timer::Update(delta_time);
        steady_clock::time_point begin = steady_clock::now();
        collisions::ChokeCheck(enemy_1, gPRECISION, gPRECISION * 0.75f, 2.0f);
        collisions::ChokeCheck(enemy_2, gPRECISION, gPRECISION * 0.75f, 2.0f);
        steady_clock::time_point end = steady_clock::now();

        utility::DebugCameraMovement(window, debugCamera, delta_time);
        gameplayCameraComponent->Update();
        input::InputManager::i_->Update();

#pragma region Collisions and Physics
        
        Timer::UpdateTimer(fixed_update_timer, delta_time);

        FixOrientation(enemy_1);
        FixOrientation(player_1);
        FixOrientation(player_2);

#pragma endregion
#pragma region GO Update and Draw

        // Bind buffer - Use Shader - Draw 
        gbuffer.Bind();
        GBufferPassShader->Use();
        GBufferPassShader->SetMatrix4("view_matrix", gameplayCameraComponent->camera_->GetViewMatrix());
        GBufferPassShader->SetMatrix4("projection_matrix", projection_matrix);

        scene_root->PropagateUpdate();
        //////////////////////////////////
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        ssao_buffer.Bind();
        SSAOShader->Use();
        ssao_buffer.BindTextures(SSAOShader, gbuffer.view_position_texture_, gbuffer.view_normal_texture_);
        ssao_buffer.Draw();
        //////////////////////////////////

        // Bind buffer - Bind textures - Use Shader - Draw 
        ssao_blur_buffer.Bind();
        SSAOBlurShader->Use();
        ssao_blur_buffer.BindTextures(SSAOBlurShader, ssao_buffer.ssao_texture_);
        ssao_blur_buffer.Draw();
        //////////////////////////////////
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        lbuffer.Bind();
        LBufferPassShader->Use();
        gbuffer.BindTextures(LBufferPassShader);
        glActiveTexture(GL_TEXTURE4);
        LBufferPassShader->SetInt("ssao_texture", 4);
        glBindTexture(GL_TEXTURE_2D, ssao_blur_buffer.texture_);
        LBufferPassShader->SetVec3("camera_position", camera->get_position());

        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS
        LBufferPassShader->SetVec3("light_positions[0]", light_Positions[0]);
        LBufferPassShader->SetVec3("light_colors[0]", light_Colors[0]);
        LBufferPassShader->SetVec3("light_positions[1]", player_1->transform_->get_position() + glm::vec3(2.0f, 2.0f, 2.0f));
        LBufferPassShader->SetVec3("light_colors[1]", light_Colors[1]);
        LBufferPassShader->SetVec3("light_positions[2]", player_2->transform_->get_position() + glm::vec3(2.0f, 2.0f, 2.0f));
        LBufferPassShader->SetVec3("light_colors[2]", light_Colors[1]);

        /*LBufferPassShader->SetVec3("camera_position", gameplayCameraComponent->camera_->get_position());
        glBindVertexArray(lbuffer.vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);*/

        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS

        lbuffer.Draw();
        //////////////////////////////////
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        BindDefault();
        PostprocessingShader->Use();
        lbuffer.BindTextures(PostprocessingShader);
        postprocessor.Draw();
        //////////////////////////////////
        
        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", gameplayCameraComponent->camera_->GetViewMatrix());
        
        cubemap->RenderCube();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        ParticleShader->Use();
        ParticleShader->SetMatrix4("view_matrix", gameplayCameraComponent->camera_->GetViewMatrix());

        ParticleEmitterManager::i_->Draw();
        
        glDisable(GL_BLEND);
        
#pragma endregion
#pragma region Interface

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        HUDshader->Use();

        HUD_object->transform_->add_rotation(glm::vec3(133.0f * delta_time, 100.0f * delta_time, 66.0f * delta_time));
        HUD_root->PropagateUpdate();
        
        HUDTextShader->Use();
        HUDTextShader->SetMatrix4("projection_matrix", ortho_matrix);
        
        HUDText_object->GetComponent<components::TextRenderer>()->text_ = "fps: " + std::to_string(1.0f / delta_time);
        HUDText_root->PropagateUpdate();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

#pragma endregion

#pragma region ImGUI
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Colors");
        ImGui::SliderFloat("Gamma", &postprocessor.gamma_, 0.1f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Brightness", &postprocessor.brightness_, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Contrast", &postprocessor.contrast_, 0.0f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::End();

ImGui::Begin("Camera");
    ImGui::SliderFloat("Pitch", &camera->pitch_, -89.0f, 89.0f, "%.2f");
    ImGui::SliderFloat("Yaw", &camera->yaw_, -179.0f, 179.0f, "%.2f");
    ImGui::SliderFloat("Height", &camera->position_.y, 1.0f, 100.0f, "%.2f");
    ImGui::SliderFloat("Distance", &gameplayCameraComponent->distance_, 1.0f, 100.0f, "%.2f");
    ImGui::SliderFloat("Yaw Angle", &gameplayCameraComponent->yawAngle_, -1.0f, 1.0f, "%.1f");



ImGui::End();

        ImGui::Begin("Generation");
        ImGui::SliderFloat("Angle", &rlgs.angle, 0.0f, 1.0f, "%0.2f");
        ImGui::SliderFloat("Span", &rlgs.span, 0.0f, 2.0f, "%0.2f");
        ImGui::SliderInt("Branch division count", &rlgs.branch_division_count, 1, 10);
        ImGui::SliderFloat("Branch division min length", &rlgs.branch_division_min_length, 1.0f, 10.0f, "%0.2f");
        ImGui::SliderFloat("Branch division max length", &rlgs.branch_division_max_length, 1.0f, 10.0f, "%0.2f");
        ImGui::SliderInt("Sub branch count", &rlgs.sub_branch_count, 0, 20);
        ImGui::SliderFloat("Sub branch span", &rlgs.sub_branch_span, 0.0f, 2.0f, "%0.2f");
        ImGui::SliderFloat("Sub branch min length", &rlgs.sub_branch_min_length, 1.0f, 10.0f, "%0.2f");
        ImGui::SliderFloat("Sub branch max length", &rlgs.sub_branch_max_length, 1.0f, 10.0f, "%0.2f");
        if (ImGui::Button("Generate"))
        {
            rlg.GenerateRooms(rlgs);
            for (auto &room : room_objects)
            {
                room.lock()->Destroy();
            }
            room_objects.clear();
            for (auto &room : rlg.rooms)
            {
                auto room_obj = GameObject::Create(scene_root);
                room_objects.push_back(room_obj);
                room_obj->AddComponent(make_shared<components::MeshRenderer>(test_ball_model, GBufferPassShader));
                room_obj->transform_->set_position(glm::vec3(room.first.x, 20.0f, room.first.y));
                room_obj->transform_->set_scale(glm::vec3(3.0f));
                room_obj->PropagateStart();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear"))
        {
            for (auto &room : room_objects)
            {
                room.lock()->Destroy();
            }
            room_objects.clear();
        }
        ImGui::End();

        ImGui::Begin("Sound");

        if (ImGui::Button("Play bruh.wav"))
        {
            audio_test_obj->GetComponent<components::AudioSource>()->PlaySound(audio::Sounds::bruh);
        }

        ImGui::End();


        ImGui::Begin("Room Generation");
        ImGui::SliderInt("Width", &rg_settings.width, 2, 10);
        ImGui::SliderInt("Height", &rg_settings.height, 2, 10);
        if (ImGui::Button("Generate"))
        {
            for (auto& a : room_parts)
            {
                a.lock()->Destroy();
            }
            room_parts.clear();
            generation::GenerateRoom(&rg_settings, &models, room_parts, scene_root, GBufferPassShader);

            pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-rg_settings.width * generation::kModuleSize, 0.0f, -rg_settings.height * generation::kModuleSize), 1.0f);
            pbd::PBDManager::i_->set_walls(walls);
        }
        ImGui::End();

        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

#pragma endregion 
        glfwSwapBuffers(window);
    }

    ai::EnemyAIManager::Destroy();
    pbd::PBDManager::Destroy();
    collisions::CollisionManager::Destroy();
    input::InputManager::Destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
