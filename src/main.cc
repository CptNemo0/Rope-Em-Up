#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <ratio>
#include <stdlib.h>

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
#include "headers/components/HealthComponent.h"
#include "headers/HDRCubemap.h"
#include "headers/Font.h"
#include "headers/components/ParticleEmitter.h"
#include "headers/ParticleEmitterManager.h"
#include "headers/generation/RoomGenerator.h"
#include "headers/audio/AudioManager.h"
#include "headers/audio/Sounds.h"
#include "headers/components/AudioSource.h"
#include "headers/CameraManager.h"

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

#include "headers/parsing/file_read.h"

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

    const string kBasicDefferedLightShaderPath = "res/shaders/BasicDefferedLight.frag";

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
    const string lFemalePlayerMeshPath = "res/models/Female/kobieta.fbx";
    const string lMalePlayerMeshPath = "res/models/Male/player_M.fbx";
    const string kDebugMeshPath = "res/models/debug_thingy.obj";
    const string kEnemyMeshPath = "res/models/enemy.obj";
    const string kTestPath = "res/models/test2.obj";
    const string kWallPath = "res/models/simple_wall.obj";
    const string kModule1Path = "res/models/module1.obj";
    const string kModule2Path = "res/models/enviroment/modules/module2.obj";
    const string kPlaceholderLampPath = "res/models/enviroment/lamps/placeholder_kamp.obj";
    const string kSimpleFloodPath = "res/models/enviroment/floor/floor.obj";
    const string kTestBallPath = "res/models/test_ball.obj";
    const string kGatePath = "res/models/gate.obj";
    const string kBonePath = "res/pierdoly/bone.obj";
    const string kLeafsPath = "res/pierdoly/leafs.obj";

    //Main modules
    const string kMod1Path = "res/Modules/mod1.obj";
    const string kMod2Path = "res/Modules/mod2.obj";
    const string kMod3Path = "res/Modules/mod3.obj";
    const string kMod4Path = "res/Modules/mod4.obj";
    const string kMod5Path = "res/Modules/mod5.obj";
    const string kMod6Path = "res/Modules/mod6.obj";
    const string kMod7Path = "res/Modules/mod7.obj";

    const string kFontPath = "res/fonts/CourierPrime-Regular.ttf";

    const string kBruhPath = "res/sounds/bruh.wav";

    const string kEnemyAiInitPath = "res/config/EnemyAiInit.ini";
    const string kVehicleInitPath = "res/config/VehicleInit.ini";
    const string kRoomLayoutGenerationSettingsInitPath = "res/config/RoomLayoutGenerationSettingsInit.ini";

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
#pragma endregion Camera Settings


    srand(static_cast <unsigned> (time(0)));
    ///// AI SETTINGS
#pragma region enemy vehicle settitngs

    Vehicle enemy_vehicle_template;
    LoadVehicleStruct(kVehicleInitPath, enemy_vehicle_template);
    ai::EnemyAIManagerInitStruct enemy_ai_init;
    LoadEnemyAiManagerInitStruct(kEnemyAiInitPath, enemy_ai_init);

#pragma endregion enemy vehicle settings

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
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 1);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(0, 2);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(2, 2);

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
    camera->set_pitch(-89.0f);
    camera->set_yaw(-89.0f);

    auto topCamera = make_shared<llr::Camera>();
    topCamera->set_fov(kFov);
    topCamera->set_near(kNear);
    topCamera->set_far(kFar);
    topCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    topCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    topCamera->set_pitch(-90.0f);
    topCamera->set_yaw(-90.0f);

    auto debugCamera = make_shared<llr::Camera>();
    debugCamera->set_fov(kFov);
    debugCamera->set_near(kNear);
    debugCamera->set_far(kFar);
    debugCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    debugCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    debugCamera->set_pitch(-90.0f);
    debugCamera->set_yaw(-90.0f);

    s_ptr<llr::Camera>* activeCamera = &camera;

#pragma endregion CamerasConfiguration

    auto projection_matrix = glm::perspective(glm::radians((*activeCamera)->get_fov()), (*activeCamera)->get_aspect_ratio(), (*activeCamera)->get_near(), (*activeCamera)->get_far());
    auto ortho_matrix = glm::ortho(0.0f, (float)mode->width, 0.0f, (float)mode->height);

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
    auto BasicDefferedLightShader = make_shared<Shader>(kLBufferVertexShaderPath, kBasicDefferedLightShaderPath);
    auto SSAOShader = make_shared<Shader>(kSSAOVertexShaderPath, kSSAOFragmentShaderPath);
    auto SSAOBlurShader = make_shared<Shader>(kSSAOBlurVertexShaderPath, kSSAOBlurFragmentShaderPath);
#pragma endregion Shaders

    auto cubemap = make_shared<HDRCubemap>(kHDREquirectangularPath, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader);
    auto HUD_texture = make_shared<tmp::Texture>(kHUDTexturePath);
    auto HUD_texture2 = make_shared<tmp::Texture>(kHUDTexturePath2);
    auto Smoke_texture = make_shared<tmp::Texture>(kTestSmokeTexturePath);

    LBuffer lbuffer = LBuffer(mode->height, mode->width);
    GBuffer gbuffer = GBuffer(mode->height, mode->width);
    SSAOBuffer ssao_buffer = SSAOBuffer(mode->height, mode->width, SSAOPrecision::LOW_SSAO);
    SSAOBlurBuffer ssao_blur_buffer = SSAOBlurBuffer(mode->height, mode->width);
    ppc::Postprocessor postprocessor = ppc::Postprocessor(mode->width, mode->height, PostprocessingShader);

    

#pragma region Lights
    PointLight point_light{};

    glm::vec3 point_light_color = glm::vec3(1.0f, 1.0f, 0.5f);
    float point_light_intensity = 1.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.color = point_light_color;

    DirectionalLight directional_light{};
    glm::vec3 dir_light_color = glm::vec3(1.0f, 1.0f, 0.5f);
    glm::vec3 dir_light_direction = glm::vec3(-0.9f, 0.8, -0.9);
    float dir_light_intensity = 0.3f;
    directional_light.direction = dir_light_direction;
    directional_light.color = dir_light_color;

    SpotLight spot_light{};
    glm::vec3 spot_light_color = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 spot_light_position = glm::vec3(-3.0f, 17.0f, -5.0f);
    glm::vec3 spot_light_direction = glm::vec3(0.0f, 0.0f, 0.0f);
    float spot_light_cut_off = glm::cos(glm::radians(12.5f));
    float spot_light_outer_cut_off = glm::cos(glm::radians(17.5f));
    float spot_light_intensity = 1.0f;
	spot_light.position = spot_light_position;
	spot_light.direction = spot_light_direction;
	spot_light.color = spot_light_color;
	spot_light.intensity = spot_light_intensity;

#pragma endregion Lights
    


    auto test_model = make_shared<Model>(kTestPath);

    auto cube_model = make_shared<Model>(kCubeMeshPath);
    auto player_model = make_shared<Model>(kPlayerMeshPath);
    /*auto F_player_model = make_shared<Model>(lFemalePlayerMeshPath);
    auto M_player_model = make_shared<Model>(lMalePlayerMeshPath)*/;
    auto debug_model = make_shared<Model>(kDebugMeshPath);
    auto enemy_model = make_shared<Model>(kEnemyMeshPath);
    auto wall_model = make_shared<Model>(kWallPath);
    auto module_1_model = make_shared<Model>(kModule1Path);
    auto module_2_model = make_shared<Model>(kModule2Path);
    auto simple_floor_model = make_shared<Model>(kSimpleFloodPath);
    auto test_ball_model = make_shared<Model>(kTestBallPath);
    auto gate_model = make_shared<Model>(kGatePath);
    auto lamp_model = make_shared<Model>(kPlaceholderLampPath);
    auto bone_model = make_shared<Model>(kBonePath);
    auto leafs_model = make_shared<Model>(kLeafsPath);

    // Main modules
    auto mod1_model = make_shared<Model>(kMod1Path);
    auto mod2_model = make_shared<Model>(kMod2Path);
    auto mod3_model = make_shared<Model>(kMod3Path);
    auto mod4_model = make_shared<Model>(kMod4Path);
    auto mod5_model = make_shared<Model>(kMod5Path);
    auto mod6_model = make_shared<Model>(kMod6Path);
    auto mod7_model = make_shared<Model>(kMod7Path);

    auto scene_root = GameObject::Create();

    generation::RoomLayoutGenerationSettings rlgs;
    LoadRoomLayoutGenerationSettingsInitStruct(kRoomLayoutGenerationSettingsInitPath, rlgs);

    generation::RoomLayoutGenerator rlg;
    std::deque<w_ptr<GameObject>> room_objects;
    rlg.GenerateRooms(rlgs);
    rlg.GenerateGates();

    for (auto& room : rlg.rooms)
    {
        auto room_obj = GameObject::Create(scene_root);
        room_objects.push_back(room_obj);
        room_obj->AddComponent(make_shared<components::MeshRenderer>(test_ball_model, GBufferPassShader));
        room_obj->transform_->set_position(glm::vec3(room.first.x, 20.0f, room.first.y));
        room_obj->transform_->set_scale(glm::vec3(3.0f));
    }

    generation::RoomModels models;
    models.walls.push_back(mod1_model);
    models.walls.push_back(mod2_model);
    models.walls.push_back(mod3_model);
    models.walls.push_back(mod4_model);
    models.walls.push_back(mod5_model);
    models.walls.push_back(mod6_model);
    models.walls.push_back(mod7_model);
    models.floors.push_back(simple_floor_model);
    models.gates.push_back(gate_model);
    models.lamps.push_back(lamp_model);
    models.clutter.push_back(bone_model);
    models.enemies.push_back(enemy_model);

    generation::RoomGenerationSettings rg_settings;
    rg_settings.width = 2;
    rg_settings.height = 2;
    rg_settings.lamps = 3;
    rg_settings.active_lamps = 2;
    rg_settings.clutter = 5;
    rg_settings.enemies = 0;

    std::deque<w_ptr<GameObject>> room_parts;

    generation::Room* room = &rlg.rooms[glm::ivec2(0, 0)];

    generation::GenerateRoom(*room, &rg_settings, &models);

    generation::BuildRoom(*room, &models, room_parts, scene_root, GBufferPassShader);
    pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
    pbd::PBDManager::i_->set_walls(walls);

    auto player_1 = GameObject::Create(scene_root);
    player_1->transform_->TeleportToPosition(glm::vec3(-0.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_1->AddComponent(make_shared<components::MeshRenderer>(player_model, GBufferPassShader));
    player_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_1->transform_));
    player_1->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_1->transform_));
    player_1->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_1));

    auto player_2 = GameObject::Create(scene_root);
    player_2->transform_->TeleportToPosition(glm::vec3(-0.7 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_2->AddComponent(make_shared<components::MeshRenderer>(player_model, GBufferPassShader));
    player_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(1, gPRECISION, player_model->meshes_[0], player_2->transform_));
    player_2->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_2->transform_));
    player_2->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_2));

    Rope rope = Rope
    (
        player_1->transform_->get_position(),
        player_2->transform_->get_position(),
        0.1251f,
        0.97f,
        scene_root,
        test_ball_model,
        GBufferPassShader
    );
    rope.AssignPlayerBegin(player_1);
    rope.AssignPlayerEnd(player_2);

    /* auto enemy_1 = GameObject::Create(scene_root);
    enemy_1->transform_->TeleportToPosition(glm::vec3(-10.0f, 0.0f, -10.0f));
    enemy_1->AddComponent(make_shared<components::MeshRenderer>(enemy_model, GBufferPassShader));
    enemy_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, enemy_model->meshes_[0], enemy_1->transform_));
    enemy_1->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy_1->transform_));
    enemy_1->AddComponent(make_shared<components::HealthComponent>(10.0f));
    enemy_1->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy_1));
    auto enemy_movement_generator_1 = make_shared<pbd::BasicGenerator>();
    pbd::PBDManager::i_->CreateFGRRecord(enemy_1->GetComponent<components::PBDParticle>(), enemy_movement_generator_1);
    auto enemy_state_machine_1 = make_shared<ai::EnemyStateMachine>(enemy_1, enemy_movement_generator_1, enemy_vehicle_template);

    auto enemy_2 = GameObject::Create(scene_root);
    enemy_2->transform_->TeleportToPosition(glm::vec3(-8.0f, 0.0f, -10.0f));
    enemy_2->AddComponent(make_shared<components::MeshRenderer>(enemy_model, GBufferPassShader));
    enemy_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, enemy_model->meshes_[0], enemy_2->transform_));
    enemy_2->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy_2->transform_));
    enemy_2->AddComponent(make_shared<components::HealthComponent>(10.0f));
    enemy_2->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy_2));
   auto enemy_movement_generator_2 = make_shared<pbd::BasicGenerator>();
    pbd::PBDManager::i_->CreateFGRRecord(enemy_2->GetComponent<components::PBDParticle>(), enemy_movement_generator_2);
    auto enemy_state_machine_2 = make_shared<ai::EnemyStateMachine>(enemy_2, enemy_movement_generator_2, enemy_vehicle_template);*/

    ai::EnemyAIManager::SetPlayers(player_1, player_2);
    ////ai::EnemyAIManager::SetEnemies(enemies) //jakis vector i potem metoda ktora go zmienia na cos innego moze zadziala
    auto isometricCamera = GameObject::Create(scene_root);
    isometricCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    isometricCamera->AddComponent(make_shared<components::GameplayCameraComponent>(player_1, player_2, camera));

    auto topDownCamera = GameObject::Create(scene_root);
    topDownCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    topDownCamera->AddComponent(make_shared<components::GameplayCameraComponent>(player_1, player_2, topCamera));

    auto isometricCameraComponent = isometricCamera->GetComponent<components::GameplayCameraComponent>();
    auto topDownCameraComponent = topDownCamera->GetComponent<components::GameplayCameraComponent>();

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
    particle_emitter->AddComponent(make_shared<components::ParticleEmitter>(100, Smoke_texture, ParticleShader, activeCamera));
    auto particle_emitter_component = particle_emitter->GetComponent<components::ParticleEmitter>();
    particle_emitter_component->emission_rate_ = 0.1f;
    particle_emitter_component->life_time_ = 1.0f;
    particle_emitter_component->start_acceleration_ = glm::vec3(0.0f, 9.81f, 0.0f);
    particle_emitter_component->start_size_ = glm::vec2(0.1f, 0.0f);
    particle_emitter_component->end_size_ = glm::vec2(0.5f, 1.0f);
    particle_emitter_component->start_position_displacement_ = 1.0f;

    auto audio_test_obj = GameObject::Create(scene_root);
    audio_test_obj->AddComponent(make_shared<components::AudioSource>());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    PBRShader->Use();
    PBRShader->SetInt("irradiance_map", 5);

    BackgroundShader->Use();
    BackgroundShader->SetInt("environmentMap", 0);

    
    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    ParticleShader->Use();
    ParticleShader->SetMatrix4("projection_matrix", projection_matrix);

    BackgroundShader->Use();
    BackgroundShader->SetMatrix4("projection_matrix", projection_matrix);

    SSAOShader->Use();
    SSAOShader->SetMatrix4("projection_matrix", projection_matrix);
    SSAOShader->SetInt("height", mode->height);
    SSAOShader->SetInt("width", mode->width);
    SSAOShader->SetInt("quality",(int)ssao_buffer.quality_);
    SSAOShader->SetFloat("radius", 0.4);
    SSAOShader->SetFloat("bias", 0.02);
    ssao_buffer.SetKernel(SSAOShader);

    cubemap->LoadHDRimg(window, *activeCamera);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    //int enemy_state_machine_1;
    //int enemy_state_machine_2;
    Timer::Timer fixed_update_timer = Timer::CreateTimer(1.0f / 120.0f, [&fixed_update_timer]()
    {
        ai::EnemyAIManager::i_->UpdateAI();
        pbd::PBDManager::i_->GeneratorUpdate();
        pbd::PBDManager::i_->Integration(pbd::kMsPerUpdate);
        collisions::CollisionManager::i_->PredictColliders();
        collisions::CollisionManager::i_->CollisionCheckPBD(pbd::PBDManager::i_->contacts_);
        pbd::PBDManager::i_->ProjectConstraints(pbd::kMsPerUpdate);
        pbd::PBDManager::i_->UpdatePositions(pbd::kMsPerUpdate);
        pbd::PBDManager::i_->ClearContacts();
        
        ParticleEmitterManager::i_->Update(pbd::kMsPerUpdate);

    }, nullptr, true);

    //////////////////////////
    // SETUP CAMERA MANAGER
    //////////////////////////

    //llr::CameraManager::debuggingCamera_ = debugCamera;
    camera->pitch_ = -45.0f;
    camera->yaw_ = 55.0f;

    isometricCameraComponent->height_ = 17.0f;
    isometricCameraComponent->distance_ = 21.0f;

    auto CameraManager = make_shared<llr::CameraManager>();
    CameraManager->setIsometricCamera(isometricCameraComponent);
    CameraManager->setTopDownCamera(topDownCameraComponent);

    // wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /////////////////////////////////////////////
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    // GAME LOOP GAME LOOP GAME LOOP GAME LOOP //
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    /////////////////////////////////////////////

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

        //cout << pbd::PBDManager::i_->particles_.size() << endl;
    
        Timer::Update(delta_time);
        steady_clock::time_point begin = steady_clock::now();
        //collisions::ChokeCheck(enemy_1, gPRECISION, gPRECISION * 0.75f, 2.0f);
        //collisions::ChokeCheck(enemy_2, gPRECISION, gPRECISION * 0.75f, 2.0f);
        steady_clock::time_point end = steady_clock::now();

        utility::DebugCameraMovement(window, debugCamera, delta_time);
        input::InputManager::i_->Update();
        audio::AudioManager::i_->Update();



#pragma region Rooms

        float gate_distance_threshold = 4.0f;
        glm::ivec2 next_room_pos = room->position;
        glm::ivec2 current_room_pos = room->position;
        
        int input_door = -1;


        if (room->up_gate)
        {
            auto p1l = glm::length2(room->up_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->up_gate_pos - player_2->transform_->get_global_position());

            if (p1l < gate_distance_threshold || p2l < gate_distance_threshold)
            {
                cout << "GO UP!!!" << endl;
                next_room_pos += glm::ivec2(0, -1);
                input_door = 0;
            }
            
        }
        if (room->right_gate)
        {
            auto p1l = glm::length2(room->right_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->right_gate_pos - player_2->transform_->get_global_position());

            if (p1l < gate_distance_threshold || p2l < gate_distance_threshold)
            {
                cout << "GO RIGHT!!!" << endl;
                next_room_pos += glm::ivec2(-1, 0);
                if (input_door == -1)
                {
                    input_door = 1;
                }
            }
        }
        if (room->down_gate)
        {
            auto p1l = glm::length2(room->down_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->down_gate_pos - player_2->transform_->get_global_position());

            if (p1l < gate_distance_threshold || p2l < gate_distance_threshold)
            {
                cout << "GO DOWN!!!" << endl;
                next_room_pos += glm::ivec2(0, 1);
                if (input_door == -1)
                {
                    input_door = 2;
                }
            }
        }
        if (room->left_gate)
        {
            auto p1l = glm::length2(room->left_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->left_gate_pos - player_2->transform_->get_global_position());

            if (p1l < gate_distance_threshold || p2l < gate_distance_threshold)
            {
                cout << "GO LEFT!!!" << endl;
                next_room_pos += glm::ivec2(1, 0);
                if (input_door == -1)
                {
                    input_door = 3;
                }
            }
        }

        if (current_room_pos != next_room_pos)
        {
            system("CLS");
            rg_settings.lamps = random::RandInt(1, 10);
            rg_settings.enemies = random::RandInt(1, 7);
            rg_settings.clutter = random::RandInt(1, 10);
            rg_settings.width = random::RandInt(1, 4);
            rg_settings.height = random::RandInt(1, 4);
            // Usun obecny pokoj
            for (auto& a : room_parts)
            {
                a.lock()->Destroy();
                a.lock() = nullptr;
            }

            room_parts.clear();
            //ai::EnemyAIManager::i_->enemy_ais_.clear();

            // Stworz nowy pokoj
            if (rlg.rooms.contains(next_room_pos))
            {
                room = &rlg.rooms[next_room_pos];

                if (!room->is_generated)
                {
                    generation::GenerateRoom(rlg.rooms[room->position], &rg_settings, &models);
                    rg_settings.width++;
                }
                generation::BuildRoom(rlg.rooms[room->position], &models, room_parts, scene_root, GBufferPassShader);

                pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
                pbd::PBDManager::i_->set_walls(walls);
            }

            //Przetransportuj gracza i line

            //przesun graczy
            glm::vec3 new_center = glm::vec3(0.0f);
            glm::vec3 player_1_pos = glm::vec3(0.0f);
            glm::vec3 player_2_pos = glm::vec3(0.0f);
            switch (input_door) 
            {
                case 0: // wychodzi gora wychodzi dolem
                    new_center = room->down_gate_pos +  glm::vec3(0.0f, 0.0f, 1.0f) * gate_distance_threshold * 1.1f;
                    player_1_pos = new_center + glm::vec3(1.0f, 0.0f, 0.0f);
                    player_2_pos = new_center - glm::vec3(1.0f, 0.0f, 0.0f);
                    player_1->transform_->TeleportToPosition(player_1_pos);
                    player_2->transform_->TeleportToPosition(player_2_pos);

                    break;
                case 1: // wychodzi od prawej wychodzi od lewej
                    new_center = room->left_gate_pos + glm::vec3(-1.0f, 0.0f, 0.0f) * gate_distance_threshold * 1.1f;
                    player_1_pos = new_center + glm::vec3(0.0f, 0.0f, 1.0f);
                    player_2_pos = new_center - glm::vec3(0.0f, 0.0f, 1.0f);
                    player_1->transform_->TeleportToPosition(player_1_pos);
                    player_2->transform_->TeleportToPosition(player_2_pos);
                    break;
                case 2: // wychodzi do�em wychodzi gora
                    new_center = room->up_gate_pos + glm::vec3(0.0f, 0.0f, -1.0f) * gate_distance_threshold * 1.1f;
                    player_1_pos = new_center + glm::vec3(1.0f, 0.0f, 0.0f);
                    player_2_pos = new_center - glm::vec3(1.0f, 0.0f, 0.0f);
                    player_1->transform_->TeleportToPosition(player_1_pos);
                    player_2->transform_->TeleportToPosition(player_2_pos);
                    break;
                case 3: // wychodzi od lewej wchodzi od prawej
                    new_center = room->right_gate_pos + glm::vec3(1.0f, 0.0f, 0.0f) * gate_distance_threshold * 1.1f;
                    player_1_pos = new_center + glm::vec3(0.0f, 0.0f, 1.0f);
                    player_2_pos = new_center - glm::vec3(0.0f, 0.0f, 1.0f);
                    player_1->transform_->TeleportToPosition(player_1_pos);
                    player_2->transform_->TeleportToPosition(player_2_pos);
                    break;
            }

            //przesun line
            auto player_distance = glm::distance(player_1->transform_->get_position(), player_2->transform_->get_position());
            glm::vec3 player_dir = glm::normalize(player_2->transform_->get_position() - player_1->transform_->get_position());
            float step = player_distance / (float)rope.Size();

            int rope_displacement_iterator = 0;
            for (auto& segment : rope.rope_segments_)
            {
                segment->transform_->TeleportToPosition(player_1->transform_->get_position() + player_dir * step * (float)rope_displacement_iterator);
                rope_displacement_iterator++;
            }
        }
        

#pragma endregion

#pragma region Collisions and Physics

        Timer::UpdateTimer(fixed_update_timer, delta_time);

#pragma endregion

#pragma region GO Update and Draw

        //glViewport(0, 0, 1920, 1080);

        // Bind buffer - Use Shader - Draw 
        gbuffer.Bind();
        GBufferPassShader->Use();
        GBufferPassShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());
        GBufferPassShader->SetMatrix4("projection_matrix", projection_matrix);

        scene_root->PropagateUpdate();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        ParticleShader->Use();
        ParticleShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());

        ParticleEmitterManager::i_->Draw();
        
        glDisable(GL_BLEND);
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
        LBufferPassShader->SetVec3("camera_position", (*activeCamera)->get_position());

        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS
        LBufferPassShader->SetInt("light_num", room->lamp_positions.size());
        //LBufferPassShader->SetFloat("intensity", 1.0f + 0.6f * std::sinf(glfwGetTime() * 0.75f));
        for (int i = 0; i < room->lamp_positions.size(); i++)
        {
            LBufferPassShader->SetVec3("pointLight[" + std::to_string(i) + "].position", glm::vec3(room->lamp_positions[i].x, 8.0f, room->lamp_positions[i].z));
            LBufferPassShader->SetVec3("pointLight[" + std::to_string(i) + "].color", point_light_color);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].constant", 1.0f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].linear", 0.045f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].quadratic", 0.0075f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].intensity", point_light_intensity /*+ 0.6f * std::sinf(glfwGetTime() * 0.75f)*/);
        }

        LBufferPassShader->SetVec3("dirLight[0].direction", dir_light_direction);
        LBufferPassShader->SetVec3("dirLight[0].color", dir_light_color);
        LBufferPassShader->SetFloat("dirLight[0].intensity", dir_light_intensity);
        ;
        LBufferPassShader->SetVec3("spotLight[0].position", spot_light_position);
        LBufferPassShader->SetVec3("spotLight[0].direction", spot_light_direction);
        LBufferPassShader->SetVec3("spotLight[0].color", spot_light_color);
        LBufferPassShader->SetFloat("spotLight[0].cutOff", spot_light_cut_off);
        LBufferPassShader->SetFloat("spotLight[0].outerCutOff", spot_light_outer_cut_off);
        LBufferPassShader->SetFloat("spotLight[0].intensity", spot_light_intensity);
        LBufferPassShader->SetFloat("spotLight[0].constant", 1.0f);
        LBufferPassShader->SetFloat("spotLight[0].linear", 0.045f);
        LBufferPassShader->SetFloat("spotLight[0].quadratic", 0.0075f);


        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS

        lbuffer.Draw();
        //////////////////////////////////
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        BindDefault();
        PostprocessingShader->Use();
        lbuffer.BindTextures(PostprocessingShader);
        PostprocessingShader->SetFloat("if_time", glfwGetTime());
       

        postprocessor.Draw();
        //////////////////////////////////
        
        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());
        
        cubemap->RenderCube();
        
#pragma endregion

#pragma region Interface

       /* glDisable(GL_DEPTH_TEST);
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
        glEnable(GL_DEPTH_TEST);*/

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
        //chose the camera
        const char* items[] = { "Isometric", "Top Down", "Debugging" };
        static int selectedItem = 1;
        ImGui::Combo("Camera Type", &selectedItem, items, IM_ARRAYSIZE(items));

            switch (selectedItem)
            {
            case 0:
                *activeCamera = isometricCameraComponent->camera_;
                break;
            case 1:
                *activeCamera = topDownCameraComponent->camera_;
                break;
            case 2:
                *activeCamera = debugCamera;
                break;
            }

        ImGui::SliderFloat("Pitch", &camera->pitch_, -89.0f, 89.0f, "%.2f");
        ImGui::SliderFloat("Yaw", &camera->yaw_, -179.0f, 179.0f, "%.2f");
        ImGui::SliderFloat("Height", &isometricCameraComponent->height_, 1.0f, 100.0f, "%.2f");
        ImGui::SliderFloat("Distance", &isometricCameraComponent->distance_, 1.0f, 100.0f, "%.2f");
        ImGui::DragFloat3("Position", glm::value_ptr((*activeCamera)->position_), 0.1f, -100.0f, 100.0f, "%.2f");
    
        /*ImGui::SliderFloat("Yaw Angle", &isometricCameraComponent->yawAngle_, -179.0f, 179.0f, "%.1f");
        ImGui::SliderFloat("Pitch Angle", &isometricCameraComponent->pitchAngle_, -89.0f, 89.0f, "%.1f");*/
        ImGui::End();

        ImGui::Begin("Lights");
        ImGui::LabelText("Point Light", "Point Light");
        ImGui::ColorEdit3("Point L Color", (float*)&point_light_color);
        ImGui::DragFloat("Point L Intensity", &point_light_intensity, 0.01f, 0.0f, 20.0f);

        ImGui::LabelText("Directional Light", "Directional Light");
        ImGui::ColorEdit3("Dir L Color", (float*)&dir_light_color);
        ImGui::DragFloat("Dir Light Intensity", &dir_light_intensity, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat3(" Dir Light Direction", glm::value_ptr(dir_light_direction), 0.05f, -1.0f, 1.0f);

        ImGui::LabelText("Spot Light", "Spot Light");
        ImGui::ColorEdit3("Spot L Color", (float*)&spot_light_color);
        ImGui::DragFloat("Spot L Intensity", &spot_light_intensity, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat3("Spot L Position", glm::value_ptr(spot_light_position), 0.05f, -100.0f, 100.0f);
        ImGui::DragFloat3("Spot L Direction", glm::value_ptr(spot_light_direction), 0.05f, -1.0f, 1.0f);
        ImGui::DragFloat("Spot L Cut Off", &spot_light_cut_off, 0.01f, 0.0f, 50.0f);
        ImGui::DragFloat("Spot L Outer Cut Off", &spot_light_outer_cut_off, 0.01f, 0.0f, 50.0f);

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
            rlg.GenerateGates();
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

        ImGui::Begin("Texture Window");
        ImVec2 textureSize(160 * 3, 90 * 3); // Adjust as per your texture size
        
        ImGui::Image((void*)(intptr_t)ssao_buffer.ssao_texture_, textureSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        ImGui::Begin("Room Generation");
        ImGui::SliderInt("Width", &rg_settings.width, 2, 10);
        ImGui::SliderInt("Height", &rg_settings.height, 2, 10);
        ImGui::SliderInt("Lamps", &rg_settings.lamps, 0, 10);
        ImGui::SliderInt("Clutter", &rg_settings.clutter, 0, 15);
        ImGui::SliderInt("Enemies", &rg_settings.enemies, 0, 15);
        if (ImGui::Button("Generate"))
        {
        //  // Usun obecny pokoj
            for (auto& a : room_parts)
            {
                a.lock()->Destroy();
            }
            room_parts.clear();
            generation::GenerateRoom(rlg.rooms[room->position], &rg_settings, &models);
            generation::BuildRoom(rlg.rooms[room->position], &models, room_parts, scene_root, GBufferPassShader);

            pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
            pbd::PBDManager::i_->set_walls(walls);
        }
        ImGui::End();

        ImGui::Begin("Rope Manager");
        ImGui::SliderFloat("Drag", &rope.segment_drag_, 0.9f, 0.999f, "%0.3f");
        ImGui::SliderFloat("Mass", &rope.segment_mass_, 0.01f, 1.0f, "%0.3f");
        if (ImGui::Button("Apply"))
        {
            rope.ApplyMass();
            rope.ApplyDrag();
        }
        if (ImGui::Button("Add Segment"))
        {
            rope.AddSegment(scene_root, test_ball_model, GBufferPassShader);
        }
        if (ImGui::Button("Remove Segment"))
        {
            rope.RemoveSegment();   
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

    audio::AudioManager::Destroy();

    return 0;
}
 