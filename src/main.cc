#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include "ft2build.h"
#include FT_FREETYPE_H
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <ratio>
#include <stdlib.h>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "stb_easy_font.h"
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
#include "headers/HealthManager.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "headers/LBuffer.h"
#include "headers/GBuffer.h"
#include "headers/SSAO.h"
#include "headers/ChokeList.h"
#include "headers/parsing/file_read.h"
#include "headers/components/Animator.h"
#include "headers/generation/Room.h"
#include "headers/drop/DropManager.h"
#include "headers/drop/SpellDropQueue.h"
#include "headers/PlayerStatsManager.h"
#include "headers/components/GrassRenderer.h"
#include "headers/GrassRendererManager.h"
#include "headers/res/Resources.h"
#include "headers/SpellCaster.h"

int main()
{
    srand(static_cast <unsigned> (time(0)));

#pragma region Resources Paths
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
    const string kPrefilterFragmentShaderPath = "res/shaders/Prefilter.frag";

    const string kBRDVertexShaderPath = "res/shaders/BRDF.vert";
    const string kBRDFragmentShaderPath = "res/shaders/BRDF.frag";

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

    const string kGrassVertexShaderPath = "res/shaders/GrassShaderVert.vert";
    const string kGrassFragmentShaderPath = "res/shaders/GrassShaderFrag.frag";

    const string kGreenTexturePath = "res/textures/green_texture.png";
    const string kRedTexturePath = "res/textures/red_texture.png";
    const string kHUDTexturePath = "res/textures/placeholder_icon.png";
    const string kHUDTexturePath2 = "res/textures/staly_elmnt.png";
    const string kTestSmokeTexturePath = "res/textures/test_smoke.png";

    const string kHDREquirectangularPath = "res/cubemaps/puresky_2k.hdr";

    const string kCubeMeshPath = "res/models/cube_2.obj";
    const string kPlayerMeshPath = "res/models/player.obj";
    const string lFemalePlayerMeshPath = "res/Players/Female/kobieta_test.fbx";
    const string kMalePlayerMeshPath = "res/Players/Male/player_M_Test_2.fbx";
    const string kDebugMeshPath = "res/models/debug_thingy.obj";
    const string kEnemyMeshPath = "res/models/enemy.obj";
    const string kTestPath = "res/models/ball.obj";
    const string kWallPath = "res/models/simple_wall.obj";
    const string kModule1Path = "res/models/module1.obj";
    const string kModule2Path = "res/models/enviroment/modules/module2.obj";
    const string kSimpleFloodPath = "res/models/enviroment/floor/floor.obj";
    const string kTestBallPath = "res/models/test_ball.obj";
    const string kGatePath = "res/models/gate.obj";
    const string kBonePath = "res/pierdoly/bone.obj";
    const string kLeafsPath = "res/pierdoly/leafs.obj";
    const string kLampPath = "res/lampa/lamp.obj";
    const string kLampCPath = "res/lampa/lamp_c.obj";

    const string kBoxPath = "res/boxes/box.obj";
    const string kBoxCPath = "res/boxes/box_c.obj";
    const string kBoxes1Path = "res/boxes/boxes1.obj";
    const string kBoxes1CPath = "res/boxes/boxes_1_c.obj";
    const string kBoxes2Path = "res/boxes/boxes2.obj";
    const string kBoxes3Path = "res/boxes/boxes3.obj";
    const string kBoxes3CPath = "res/boxes/boxes_3_c.obj";

    //Main modules
    const string kMod1Path = "res/Modules/mod1.obj";
    const string kMod2Path = "res/Modules/mod2.obj";
    const string kMod3Path = "res/Modules/mod3.obj";
    const string kMod4Path = "res/Modules/mod4.obj";
    const string kMod5Path = "res/Modules/mod5.obj";
    const string kMod6Path = "res/Modules/mod6.obj";
    const string kMod7Path = "res/Modules/mod7.obj";

    const string kGrassPath = "res/grass/grass.obj";

    const string kFontPath = "res/fonts/CourierPrime-Regular.ttf";

    const string kBruhPath = "res/sounds/tentaclepull.wav";

    const string kEnemyAiInitPath = "res/config/EnemyAiInit.ini";
    const string kVehicleInitPath = "res/config/VehicleInit.ini";
    const string kRoomLayoutGenerationSettingsInitPath = "res/config/RoomLayoutGenerationSettingsInit.ini";
    const string kRoomGenerationSettingsInitPath = "res/config/RoomGenerationSettingsInit.ini";
    const string kPBDManagerInitSettingsPath = "res/config/PBDManagerInitSettings.ini";

    const string kTentaclPath = "res/enemy/enemy.obj";
#pragma endregion Resources Paths
    
#pragma region CameraSettings
    const float kFov = 90.0f;
    const float kNear = 0.1f;
    const float kFar = 1000.0f;
    float kpitch = -90.0f;
    float kyaw = 90.0f;
#pragma endregion Camera Settings

#pragma region Loading Settings

    Vehicle enemy_vehicle_template;
    ai::EnemyAIManagerInitStruct enemy_ai_init;
    generation::RoomLayoutGenerationSettings rlgs;
    generation::RoomGenerationSettings rg_settings;
    pbd::PBDManagerInitStruct pbd_settings;
    LoadVehicleStruct(kVehicleInitPath, enemy_vehicle_template);
    LoadEnemyAiManagerInitStruct(kEnemyAiInitPath, enemy_ai_init);
    LoadRoomLayoutGenerationSettingsInitStruct(kRoomLayoutGenerationSettingsInitPath, rlgs);
    LoadRoomGenerationSettingsInitStruct(kRoomGenerationSettingsInitPath, rg_settings);
    LoadPBDManagerInitStruct(kPBDManagerInitSettingsPath, pbd_settings);

#pragma endregion Loading Settings

#pragma region Initialization

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

    collisions::CollisionManager::Initialize();

    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::PLAYER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::LAMPS);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::LAMPS);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::LAMPS);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::CLUTTER, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::CLUTTER, collisions::LAYERS::LAMPS);

    pbd::PBDManager::Initialize(pbd_settings);
    ai::EnemyAIManager::Initialize(enemy_ai_init, enemy_vehicle_template);
    ParticleEmitterManager::Initialize();
    HealthManager::Initialize();
    audio::AudioManager::Initialize();
    audio::AudioManager::i_->LoadSound(audio::Sounds::bruh, kBruhPath);
    drop::DropManager::Initialize();
    drop::SpellDropQueue::Initialize();
    ChokeList::Initialize();
    SpellCaster::Initialize();
    res::init_freetype();
    

#pragma endregion Initialization
    
#pragma region CamerasConfiguration
    auto camera = make_shared<llr::Camera>();
    camera->set_fov(kFov);
    camera->set_near(kNear);
    camera->set_far(kFar);
    camera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    camera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));

    auto topCamera = make_shared<llr::Camera>();
    topCamera->set_fov(kFov);
    topCamera->set_near(kNear);
    topCamera->set_far(kFar);
    topCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    topCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));

    auto debugCamera = make_shared<llr::Camera>();
    debugCamera->set_fov(kFov);
    debugCamera->set_near(kNear);
    debugCamera->set_far(kFar);
    debugCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    debugCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    debugCamera->set_pitch(-90.0f);
    debugCamera->set_yaw(-90.0f);

    s_ptr<llr::Camera>* activeCamera = &camera;

    input::InputManager::Initialize(window, activeCamera);

    auto projection_matrix = glm::perspective(glm::radians((*activeCamera)->get_fov()), (*activeCamera)->get_aspect_ratio(), (*activeCamera)->get_near(), (*activeCamera)->get_far());
    auto ortho_matrix = glm::ortho(0.0f, (float)mode->width, 0.0f, (float)mode->height);

#pragma endregion CamerasConfiguration

#pragma region Shaders
    auto HUDshader = res::get_shader(kHUDVertexShaderPath, kHUDFragmentShaderPath);
    auto HUDTextShader = res::get_shader(kHUDTextVertexShaderPath, kHUDTextFragmentShaderPath);
    auto PBRShader = res::get_shader(kPBRVertexShaderPath, kPBRFragmentShaderPath);
    auto EquirectangularToCubemapShader = res::get_shader(kHDRCubemapVertexShaderPath, kHDREquirectangularToCubemapFragmentShaderPath);
    auto BackgroundShader = res::get_shader(kBackgroundVertexShaderPath, kBackgroundFragmentShaderPath);
    auto IrradianceShader = res::get_shader(kHDRCubemapVertexShaderPath, kIrradianceFragmentShaderPath);
    auto PrefilterShader = res::get_shader(kHDRCubemapVertexShaderPath, kPrefilterFragmentShaderPath);
    auto BRDFShader = res::get_shader(kBRDVertexShaderPath, kBRDFragmentShaderPath);
    auto ParticleShader = res::get_shader(kParticleVertexShaderPath, kParticleGeometryShaderPath, kParticleFragmentShaderPath);
    auto PostprocessingShader = res::get_shader(kPostprocessingVertexShaderPath, kPostprocessingFragmentShaderPath);
    auto GBufferPassShader = res::get_shader(kGBufferVertexShaderPath, kGBufferFragmentShaderPath);
    auto LBufferPassShader = res::get_shader(kLBufferVertexShaderPath, kLBufferFragmentShaderPath);
    auto BasicDefferedLightShader = res::get_shader(kLBufferVertexShaderPath, kBasicDefferedLightShaderPath);
    auto SSAOShader = res::get_shader(kSSAOVertexShaderPath, kSSAOFragmentShaderPath);
    auto SSAOBlurShader = res::get_shader(kSSAOBlurVertexShaderPath, kSSAOBlurFragmentShaderPath);
    auto GrassShader = res::get_shader(kGrassVertexShaderPath, kGrassFragmentShaderPath);
#pragma endregion Shaders

    auto cubemap = make_shared<HDRCubemap>(kHDREquirectangularPath, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader, PrefilterShader, BRDFShader);
    auto HUD_texture = res::get_texture(kHUDTexturePath);
    auto HUD_texture2 = res::get_texture(kHUDTexturePath2);
    auto Smoke_texture = res::get_texture(kTestSmokeTexturePath);

    LBuffer lbuffer = LBuffer(mode->height, mode->width);
    GBuffer gbuffer = GBuffer(mode->height, mode->width);
    SSAOBuffer ssao_buffer = SSAOBuffer(mode->height, mode->width, SSAOPrecision::HIGH_SSAO);
    SSAOBlurBuffer ssao_blur_buffer = SSAOBlurBuffer(mode->height, mode->width);
    ppc::Postprocessor postprocessor = ppc::Postprocessor(mode->width, mode->height, PostprocessingShader);

#pragma region Lights
    PointLight point_light{};

    float lamp_h = 8.0f;
    glm::vec3 point_light_color = glm::vec3(1.0f, 1.0f, 0.5f);
    float point_light_intensity = 250.0f;
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.color = point_light_color;

    DirectionalLight directional_light{};
    glm::vec3 dir_light_color = glm::vec3(1.0f, 1.0f, 1.f);
    glm::vec3 dir_light_direction = glm::vec3(0.0f, -1.0f, -1.0f);
    float dir_light_intensity = 0.38f;
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

#pragma region Models
    auto test_model = res::get_model(kTestPath);

    auto cube_model = res::get_model(kCubeMeshPath);
    auto player_model = res::get_model(kPlayerMeshPath);
    //auto F_player_model = res::get_model(lFemalePlayerMeshPath);
    auto M_player_model = res::get_model(kMalePlayerMeshPath);
    auto debug_model = res::get_model(kDebugMeshPath);
    auto enemy_model = res::get_model(kTentaclPath);
    auto wall_model = res::get_model(kWallPath);
    auto module_1_model = res::get_model(kModule1Path);
    auto module_2_model = res::get_model(kModule2Path);
    auto simple_floor_model = res::get_model(kSimpleFloodPath);
    auto test_ball_model = res::get_model(kTestBallPath);
    auto gate_model = res::get_model(kGatePath);
    auto lamp_model = res::get_model(kLampPath);
    auto lamp_c_model = res::get_model(kLampCPath);

    auto bone_model = res::get_model(kBonePath);
    auto leafs_model = res::get_model(kLeafsPath);
    auto box_model = res::get_model(kBoxPath);
    auto box_c_model = res::get_model(kBoxCPath);
    auto boxes_1_model = res::get_model(kBoxes1Path);
    auto boxes_1_c_model = res::get_model(kBoxes1CPath);
    auto boxes_2_model = res::get_model(kBoxes2Path);
    auto boxes_3_model = res::get_model(kBoxes3Path);
    auto boxes_3_c_model = res::get_model(kBoxes3CPath);

    auto grass_model = res::get_model(kGrassPath);

    // Main modules
    auto mod1_model = res::get_model(kMod1Path);
    auto mod2_model = res::get_model(kMod2Path);
    auto mod3_model = res::get_model(kMod3Path);
    auto mod4_model = res::get_model(kMod4Path);
    auto mod5_model = res::get_model(kMod5Path);
    auto mod6_model = res::get_model(kMod6Path);
    auto mod7_model = res::get_model(kMod7Path);

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
    models.lamps_c.push_back(lamp_c_model);
    models.clutter.push_back(bone_model);
    models.clutter_c.push_back(bone_model);
    models.clutter.push_back(box_model);
    models.clutter_c.push_back(box_c_model);
    models.clutter.push_back(boxes_1_model);
    models.clutter_c.push_back(boxes_1_c_model);
    models.clutter.push_back(boxes_3_model);
    models.clutter_c.push_back(boxes_3_c_model);
    models.enemies.push_back(enemy_model);

    GrassRendererManager::Initialize(grass_model);

    // Spell Drops
    auto place_holder_drop_model = res::get_model(kDebugMeshPath);

    drop::SpellDropQueue::i_->drop_meshes.push_back(place_holder_drop_model);
    drop::SpellDropQueue::i_->shader = GBufferPassShader;
    
    for (int i = 0; i < 1000; i++)
    {
        drop::SpellDropQueue::i_->queue_.push(SPELLS::SKULL_MINION);
    }
    
#pragma endregion Models
    
    auto scene_root = GameObject::Create();
    auto room_root = GameObject::Create(scene_root);

    generation::RoomLayoutGenerator rlg;
    std::deque<w_ptr<GameObject>> room_objects;
    std::deque<w_ptr<GameObject>> room_parts;
    std::vector<w_ptr<GameObject>> enemies_parts;

    rlg.GenerateRooms(rlgs, room_root);
    rlg.GenerateGates();

    for (auto& room : rlg.rooms)
    {
        auto room_obj = GameObject::Create(scene_root);
        room_objects.push_back(room_obj);
        room_obj->AddComponent(make_shared<components::MeshRenderer>(test_ball_model, GBufferPassShader));
        room_obj->transform_->set_position(glm::vec3(room.first.x, 20.0f, room.first.y));
        room_obj->transform_->set_scale(glm::vec3(3.0f));
    }

    generation::Room* room = &rlg.rooms[glm::ivec2(0, 0)];
    generation::GenerateRoom(*room, &rg_settings, &models);
    generation::BuildRoom(*room, &models, GBufferPassShader);
    pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
    pbd::PBDManager::i_->set_walls(walls);

    auto player_1 = GameObject::Create(scene_root);
    player_1->transform_->TeleportToPosition(glm::vec3(-0.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_1->AddComponent(make_shared<components::MeshRenderer>(M_player_model, GBufferPassShader));
    player_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::PLAYER, gPRECISION, M_player_model, 0, player_1->transform_));
    player_1->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_1->transform_));
    player_1->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_1));
    player_1->AddComponent(HealthManager::i_->CreateHealthComponent(100.0f, PLAYER));
    player_1->AddComponent(make_shared<components::SpellSlotComponent>(components::SSC_INIT::NO_SPELL));
    player_1->GetComponent<components::SpellSlotComponent>()->type_ = SKULL_MINION;
    player_1->GetComponent<components::SpellSlotComponent>()->spell_levels[SKULL_MINION] = 3;

    auto player_2 = GameObject::Create(scene_root);
    player_2->transform_->TeleportToPosition(glm::vec3(-0.7 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_2->AddComponent(make_shared<components::MeshRenderer>(M_player_model, GBufferPassShader));
    player_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::PLAYER, gPRECISION, M_player_model, 0, player_2->transform_));
    player_2->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_2->transform_));
    player_2->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_2));
    player_2->AddComponent(HealthManager::i_->CreateHealthComponent(100.0f, PLAYER));
    player_2->AddComponent(make_shared<components::SpellSlotComponent>(components::SSC_INIT::NO_SPELL));
    player_2->GetComponent<components::SpellSlotComponent>()->type_ = SKULL_MINION;
    player_2->GetComponent<components::SpellSlotComponent>()->spell_levels[SKULL_MINION] = 22;

    std::vector<std::shared_ptr<GameObject>> players_vector {player_1, player_2};

    /*auto test_ball = GameObject::Create(scene_root);
    test_ball->transform_->set_scale(glm::vec3(0.2f));
    test_ball->transform_->set_position(player_2->transform_->get_position() + glm::vec3(1, 3, -2));
    test_ball->AddComponent(make_shared<components::MeshRenderer>(test_model, GBufferPassShader));
    test_ball->transform_->add_rotation(glm::vec3(-90.0f, 0.0f, 0.0f));*/
    
    auto test_animation = make_shared<anim::Animation>(kMalePlayerMeshPath, M_player_model); 
    player_1->AddComponent(make_shared<components::Animator>(test_animation));
    player_1->transform_->set_scale(glm::vec3(0.01f));
    player_2->transform_->set_scale(glm::vec3(0.01f));

    Rope rope = Rope
    (
        player_1->transform_->get_position(),
        player_2->transform_->get_position(),
        0.125f,
        0.97f,
        scene_root,
        test_ball_model,
        GBufferPassShader
    );


    rope.AssignPlayerBegin(player_1);
    rope.AssignPlayerEnd(player_2);

    for (int i = 0; i < 20; i++)
    {
        rope.AddSegment(scene_root);
    }

    ai::EnemyAIManager::SetPlayers(player_1, player_2);
    PlayerStatsManager::Initialize(&rope, player_1, player_2);


#pragma region Camera
    auto isometricCamera = GameObject::Create(scene_root);
    isometricCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    auto isometricCameraComp = make_shared<components::CameraComponent>(player_1, player_2, camera);
    isometricCamera->AddComponent(isometricCameraComp);
    isometricCameraComp->distanceX_ = -7.0f;
    isometricCameraComp->distanceZ_ = -7.0f;
    isometricCameraComp->height_ = 10.0f;

    auto topDownCamera = GameObject::Create(scene_root);
    topDownCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    topDownCamera->AddComponent(make_shared<components::CameraComponent>(player_1, player_2, topCamera));

    auto DebugCamera = GameObject::Create(scene_root);
    DebugCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    DebugCamera->AddComponent(make_shared<components::CameraComponent>(debugCamera));


    auto isometricCameraComponent = isometricCamera->GetComponent<components::CameraComponent>();
    auto topDownCameraComponent = topDownCamera->GetComponent<components::CameraComponent>();
    auto DebugCameraComponent = DebugCamera->GetComponent<components::CameraComponent>();
#pragma endregion Camera


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

    auto maturasc_font = res::get_font(kFontPath);

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
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


    GBufferPassShader->Use();
    unsigned int maxBones = MAX_BONES;
    unsigned int ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxBones * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    LBufferPassShader->Use();
    LBufferPassShader->SetInt("irradianceMap", 8);
    LBufferPassShader->SetInt("prefilterMap", 9);
    LBufferPassShader->SetInt("brdfLUT", 10);

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
    SSAOShader->SetFloat("radius", 0.2);
    SSAOShader->SetFloat("bias", 0.01);
    ssao_buffer.SetKernel(SSAOShader);

    // cubemap->LoadHDRimg(window, *activeCamera);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    /*int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);*/

    float fixed_update_rate = pbd::kMsPerUpdate;
    Timer::Timer fixed_update_timer = Timer::CreateTimer(1.0f / 120.0f, [&fixed_update_timer, &fixed_update_rate, player_1, player_2]()
    {
        ai::EnemyAIManager::i_->UpdateAI();
        pbd::PBDManager::i_->GeneratorUpdate();
        pbd::PBDManager::i_->Integration(fixed_update_rate);
        pbd::PBDManager::i_->ClearContacts();
        collisions::CollisionManager::i_->PredictColliders();
        collisions::CollisionManager::i_->CollisionCheckPBD(pbd::PBDManager::i_->contacts_);
        pbd::PBDManager::i_->ProjectConstraints(fixed_update_rate);
        pbd::PBDManager::i_->UpdatePositions(fixed_update_rate);
        ParticleEmitterManager::i_->Update(fixed_update_rate);

    }, nullptr, true);

    
    //auto grass = GameObject::Create(scene_root);
    //grass->AddComponent(GrassRendererManager::i_->CreateRenderer(walls.up_left_, walls.down_right_, 600));


    /////////////////////////////////////////////
    /////////////////////////////////////////////
    // GAME LOOP GAME LOOP GAME LOOP GAME LOOP //
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
    
        Timer::Update(delta_time);

        player_1->GetComponent<components::Animator>()->SetDeltaTime(delta_time);
        utility::DebugCameraMovement(window, DebugCameraComponent->camera_, delta_time);
        input::InputManager::i_->Update();
        audio::AudioManager::i_->Update();

#pragma region Rooms

        static bool moving_through_room = false;

        if (!moving_through_room)
        {

            rg_settings.width = random::RandInt(1, 2);
            rg_settings.height = random::RandInt(1, 2);
            rg_settings.enemies = random::RandInt(1, 5);
            rg_settings.lamps = random::RandInt(1, 5);
            rg_settings.clutter = random::RandInt(1, 5);

            glm::ivec2 current_room_pos = room->position;
            glm::ivec2 move_direction = generation::GetMoveDirection(room, player_1, player_2);
            glm::ivec2 next_room_pos = current_room_pos + move_direction;

            if ((current_room_pos != next_room_pos) && rlg.rooms.contains(next_room_pos))
            {
                cout << "GOING THROUGH ROOM";
                // Temporarily stop players and player inputs
                moving_through_room = true;
                auto pc1 = player_1->GetComponent<components::PlayerController>();
                pc1->active_ = false;
                pc1->direction_ = glm::vec3(0.0f);
                pc1->move_generator_->direction_ = glm::vec3(0.0f);
                auto pc2 = player_2->GetComponent<components::PlayerController>();
                pc2->active_ = false;
                pc2->direction_ = glm::vec3(0.0f);
                pc2->move_generator_->direction_ = glm::vec3(0.0f);

                Timer::AddTimer(postprocessor.transition_vignette_time_, [&room, &rlg, &rg_settings, &models, &next_room_pos, &GBufferPassShader, &move_direction, &player_1, &player_2, &rope, &pc1, &pc2, &postprocessor]()
                {
                    generation::ChangeRooms(room, rlg, rg_settings, models, next_room_pos, GBufferPassShader);
                    generation::DisplacePlayersAndRope(room, move_direction, player_1, player_2, rope);
                    pc1->active_ = true;
                    pc2->active_ = true;
                    postprocessor.transition_vignette_current_time_ = postprocessor.transition_vignette_time_;

                    Timer::AddTimer(postprocessor.transition_vignette_time_, [&postprocessor]()
                    {
                        postprocessor.transition_vignette_current_time_ = 0.0f;
                        moving_through_room = false;
                    },
                    [&postprocessor](float delta_time)
                    {
                        postprocessor.transition_vignette_current_time_ -= delta_time;
                    }, false);
                },
                [&postprocessor](float delta_time)
                {
                    postprocessor.transition_vignette_current_time_ += delta_time;
                }, false);
            }
        }
       
#pragma endregion

#pragma region Collisions and Physics
  
        rope.ChokeCheck(room);
        Timer::UpdateTimer(fixed_update_timer, delta_time);
        HealthManager::i_->DeathUpdate();
        
        // glfwGetKey(window, GLFW_KEY_SPACE
        // HealthManager::i_->something_died_
        
        static float cast_time = 2.0f;
        static float slowdown_smooth_factor = 0.055f;
        if (HealthManager::i_->something_died_ && HealthManager::i_->what_ == MONSTER)
        {
            auto ss1 = player_1->GetComponent<components::SpellSlotComponent>()->type_;
            auto ss2 = player_2->GetComponent<components::SpellSlotComponent>()->type_;

            if(!(ss1 == NOT_A_SPELL && ss2 == NOT_A_SPELL))
            cout << HealthManager::i_->what_ << " " << HealthManager::i_->where_.x << " " << HealthManager::i_->where_.z << endl;

            postprocessor.slowed_time = true;
            SpellCaster::i_->active_ = true;
            Timer::Timer spell_timer = Timer::AddTimer(cast_time,
            [&fixed_update_rate, &postprocessor]()
            {
                fixed_update_rate = pbd::kMsPerUpdate;
                postprocessor.slowed_time = false;
            },

            [&fixed_update_rate, &id = spell_timer.id, &window, &postprocessor](float delta_time)
            {
                fixed_update_rate = fixed_update_rate * (1.0f - slowdown_smooth_factor) + 0.0000000001f * slowdown_smooth_factor;

                if (!SpellCaster::i_->active_)
                {
                    fixed_update_rate = pbd::kMsPerUpdate;
                    postprocessor.slowed_time = false;
                    SpellCaster::i_->Cast();
                    Timer::RemoveTimer(id);
                }

            },

            false);
                
        }

        drop::DropManager::i_->DropHp(players_vector);
        drop::DropManager::i_->DropSpells(*room);
        drop::DropManager::i_->DropExp();

#pragma endregion

#pragma region GO Update and Draw

        glViewport(0, 0, mode->width, mode->height);

        // Bind buffer - Use Shader - Draw 
        gbuffer.Bind();

        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());
        cubemap->BindEnvCubemap(BackgroundShader);
        cubemap->RenderCube();

        GrassShader->Use();
        GrassShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());
        GrassShader->SetMatrix4("projection_matrix", projection_matrix);
        GrassShader->SetFloat("time", glfwGetTime());
        GrassShader->SetVec3("pp1", player_1->transform_->get_position());
        GrassShader->SetVec3("pp2", player_2->transform_->get_position());
        GrassRendererManager::i_->Draw(GrassShader->get_id());


        GBufferPassShader->Use();
        GBufferPassShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());
        GBufferPassShader->SetMatrix4("projection_matrix", projection_matrix);
        GBufferPassShader->SetInt("numBones", maxBones);
        auto transforms = player_1->GetComponent<components::Animator>()->GetFinalBoneMatrices();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, transforms.size() * sizeof(glm::mat4), transforms.data());

        scene_root->PropagateUpdate();

        //////////////////////////////////
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        ssao_buffer.Bind();
        SSAOShader->Use();
        ssao_buffer.BindTextures(SSAOShader, gbuffer.view_position_texture_, gbuffer.view_normal_texture_, gbuffer.mask_texture_);
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
        cubemap->BindIrradianceMap(LBufferPassShader);
        cubemap->BindIBLmaps(LBufferPassShader);

        glActiveTexture(GL_TEXTURE4);
        LBufferPassShader->SetInt("ssao_texture", 4);
        glBindTexture(GL_TEXTURE_2D, ssao_blur_buffer.texture_);

        //glActiveTexture(GL_TEXTURE8);
        //LBufferPassShader->SetInt("mask_texture", 8);
        //glBindTexture(GL_TEXTURE_2D, gbuffer.mask_texture_);
        LBufferPassShader->SetVec3("camera_position", (*activeCamera)->get_position());

        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS
        LBufferPassShader->SetInt("light_num", room->lamp_positions.size());
        //LBufferPassShader->SetFloat("intensity", poin 1.0f + 0.6f * std::sinf(glfwGetTime() * 0.75f));
        for (int i = 0; i < room->lamp_positions.size(); i++)
        {
            LBufferPassShader->SetVec3("pointLight[" + std::to_string(i) + "].position", glm::vec3(room->lamp_positions[i].x, lamp_h, room->lamp_positions[i].z));
            LBufferPassShader->SetVec3("pointLight[" + std::to_string(i) + "].color", point_light_color);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].constant", 1.0f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].linear", 0.00f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].quadratic", 1.0f);
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].intensity", point_light_intensity);
        }

        LBufferPassShader->SetVec3("dirLight[0].direction", dir_light_direction);
        LBufferPassShader->SetVec3("dirLight[0].color", dir_light_color);
        LBufferPassShader->SetFloat("dirLight[0].intensity", dir_light_intensity);
        
        LBufferPassShader->SetVec3("spotLight[0].position", spot_light_position);
        LBufferPassShader->SetVec3("spotLight[0].direction", spot_light_direction);
        LBufferPassShader->SetVec3("spotLight[0].color", spot_light_color);
        LBufferPassShader->SetFloat("spotLight[0].cutOff", spot_light_cut_off);
        LBufferPassShader->SetFloat("spotLight[0].outerCutOff", spot_light_outer_cut_off);
        LBufferPassShader->SetFloat("spotLight[0].intensity", spot_light_intensity);
        LBufferPassShader->SetFloat("spotLight[0].constant", 1.0f);
        LBufferPassShader->SetFloat("spotLight[0].linear", 0.09);
        LBufferPassShader->SetFloat("spotLight[0].quadratic", 0.032f);
        LBufferPassShader->SetBool("slowed_time", postprocessor.slowed_time);
        if (lbuffer.bloom_)
        {
            LBufferPassShader->SetBool("bloom", lbuffer.bloom_);
            LBufferPassShader->SetFloat("bloom_threshold", lbuffer.bloom_threshold_);
            LBufferPassShader->SetVec3("bloom_color", lbuffer.bloom_color_);
        }
        else
        {
            LBufferPassShader->SetBool("bloom", lbuffer.bloom_);
        }

        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS
        lbuffer.Draw();
        //////////////////////////////////

        // FORWARD PASS - FORWARD PASS - FORWARD PASS - FORWARD PASS
    
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.g_buffer_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lbuffer.l_buffer_);
        glBlitFramebuffer(0, 0, mode->width, mode->height, 0, 0, mode->width, mode->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        ParticleShader->Use();
        ParticleShader->SetMatrix4("view_matrix", (*activeCamera)->GetViewMatrix());

        ParticleEmitterManager::i_->Draw();

        glDisable(GL_BLEND);

        
        // Bind buffer - Bind textures - Use Shader - Draw 
        postprocessor.Bind();
        PostprocessingShader->Use();
        lbuffer.BindTextures(PostprocessingShader);
        PostprocessingShader->SetFloat("if_time", glfwGetTime());
        
        postprocessor.Draw();
        //////////////////////////////////

#pragma endregion

#pragma region Interface

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        /*HUDshader->Use();

        HUD_object->transform_->add_rotation(glm::vec3(133.0f * delta_time, 100.0f * delta_time, 66.0f * delta_time));
        HUD_root->PropagateUpdate();*/
        
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

        ImGui::Begin("Postprocessor");
        ImGui::SliderFloat("Gamma", &postprocessor.gamma_, 0.1f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Brightness", &postprocessor.brightness_, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Contrast", &postprocessor.contrast_, 0.0f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Vignete Contrast", &postprocessor.vignete_contrast_, 0.0f, 200.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Vignete Amount", &postprocessor.vignete_amount_, 0.0f, 0.5f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("Noise Amount", &postprocessor.noise_amount_, 0.0f, 0.5f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::End();

        ImGui::Begin("Camera");
        //chose the camera
        const char* items[] = { "Isometric", "Top Down", "Debugging" };
        static int selectedItem = 0;
        ImGui::Combo("Camera Type", &selectedItem, items, IM_ARRAYSIZE(items));

            switch (selectedItem)
            {
            case 0:
                *activeCamera = isometricCameraComponent->camera_;
                //isometricCameraComponent->transform_->set_position((*activeCamera)->position_);
                ImGui::SliderFloat("Y", &isometricCameraComponent->height_, 1.0f, 20.0f, "%.0f");
                ImGui::SliderFloat("X", &isometricCameraComponent->distanceX_, -10.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("Z", &isometricCameraComponent->distanceZ_, -10.0f, 10.0f, "%.0f");
                break;
            case 1:
                *activeCamera = topDownCameraComponent->camera_;
                //topDownCameraComponent->transform_->set_position((*activeCamera)->position_);
                ImGui::SliderFloat("Y", &topDownCameraComponent->height_, 1.0f, 100.0f, "%.2f");
                break;
            case 2:
                *activeCamera = DebugCameraComponent->camera_;
                //DebugCameraComponent->transform_->set_position((*activeCamera)->position_);
                break;
            }


        ImGui::DragFloat3("Position", glm::value_ptr((*activeCamera)->position_), 0.1f, -100.0f, 100.0f, "%.2f");
    
        ImGui::End();

        ImGui::Begin("Lights");
        ImGui::LabelText("Point Light", "Point Light");
        ImGui::ColorEdit3("Point L Color", (float*)&point_light_color);
        ImGui::DragFloat("Point L Intensity", &point_light_intensity, 0.01f, 0.0f, 1000.0f);
        ImGui::SliderFloat("Height", &lamp_h, 0.0f, 10.0f, "%0.2f");

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

        ImGui::Checkbox("Bloom", &lbuffer.bloom_);
        ImGui::ColorEdit3("Bloom Color", (float*)&lbuffer.bloom_color_);
        ImGui::SliderFloat("Bloom Threshold", &lbuffer.bloom_threshold_, 0.0f, 1.0f, "%0.2f");

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
            rlg.GenerateRooms(rlgs, scene_root);
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
        
        ImGui::Image((void*)(intptr_t)gbuffer.world_position_texture_, textureSize, ImVec2(0, 1), ImVec2(1, 0));
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
            rope.AddSegment(scene_root);
        }
        if (ImGui::Button("Remove Segment"))
        {
            rope.RemoveSegment();   
        }
        ImGui::End();
        ImGui::Begin("Disable test");
        static bool cb = true;
        if (ImGui::Checkbox("Enabled", &cb))
        {
            if (cb)
            {
                player_1->Enable();
            }
            else
            {
                player_1->Disable();
            }
        }
        ImGui::End();

        ImGui::Begin("Player Stats Manager");
        ImGui::SliderFloat("Exp", &(PlayerStatsManager::i_->exp_), 0.0f, 1000.0f, "%0.0f");
        ImGui::SameLine();
        if (ImGui::Button("Add Exp"))
        {
            PlayerStatsManager::i_->AddExp(100.0f);
        }
        ImGui::SliderInt("Unspent Levels", &(PlayerStatsManager::i_->unspent_levels_), 0, 20);

        ImGui::SliderFloat("Speed", &(PlayerStatsManager::i_->speed_), 0.0f, 2000.0f, "%0.0f");
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Speed"))
        {
            PlayerStatsManager::i_->LevelUpSpeed();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Speed"))
        {
            PlayerStatsManager::i_->LevelDownSpeed();
        }

        ImGui::SliderFloat("Pull", &(PlayerStatsManager::i_->pull_power_), 0.0f, 3000.0f, "%0.0f");
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Pull"))
        {
            PlayerStatsManager::i_->LevelUpPull();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Pull"))
        {
            PlayerStatsManager::i_->LevelDownPull();
        }

        ImGui::SliderFloat("Intertia", &(PlayerStatsManager::i_->rope_drag_), 0.0f, 1.0f, "%0.5f");
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Intertia"))
        {
            PlayerStatsManager::i_->LevelUpDrag();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Intertia"))
        {
            PlayerStatsManager::i_->LevelDownDrag();
        }

        ImGui::SliderFloat("Rope weight", &(PlayerStatsManager::i_->rope_weight_), 0.0f, 1.0f, "%0.5f");
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Weight"))
        {
            PlayerStatsManager::i_->LevelUpWeight();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Weight"))
        {
            PlayerStatsManager::i_->LevelDownWeight();
        }

        ImGui::SliderFloat("Max Health", &(PlayerStatsManager::i_->max_health_), 0.0f, 1000.0f, "%0.1f");
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Health"))
        {
            PlayerStatsManager::i_->LevelUpHealth();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Health"))
        {
            PlayerStatsManager::i_->LevelDownHealth();
        }

        ImGui::SliderInt("Segment number", &(PlayerStatsManager::i_->segments_num_), 0, 100);
        ImGui::SameLine();
        if (ImGui::Button("LevelUp Segment number"))
        {
            PlayerStatsManager::i_->LevelUpSegments();
        }
        ImGui::SameLine();
        if (ImGui::Button("LevelDown Segment number"))
        {
            PlayerStatsManager::i_->LevelDownSegments();
        }

        if (ImGui::Button("Apply Level"))
        {
            PlayerStatsManager::i_->Apply();
        }
        ImGui::End();

        static float player_1_hp;
        static float player_2_hp;
        player_1_hp = player_1->GetComponent<components::HealthComponent>()->health_;
        player_2_hp = player_2->GetComponent<components::HealthComponent>()->health_;
        ImGui::Begin("Player 1 HP");
        ImGui::SliderFloat("Player 1 HP", &(player_1_hp), 0.0f, player_1->GetComponent<components::HealthComponent>()->max_health_, "%0.1f");
        ImGui::End();

        ImGui::Begin("Player 2 HP");
        ImGui::SliderFloat("Player 2 HP", &(player_2_hp), 0.0f, player_2->GetComponent<components::HealthComponent>()->max_health_, "%0.1f");
        ImGui::End();

        static glm::vec3 bot_color;
        static glm::vec3 top_color;
        static float offset;
        GrassShader->Use();
        ImGui::Begin("Grass");
        if (ImGui::ColorEdit3("bot_color", (float*)&bot_color))
        {
            GrassShader->SetVec3("bot_color", bot_color);
        }
        if (ImGui::ColorEdit3("top_color", (float*)&top_color))
        {
            GrassShader->SetVec3("top_color", top_color);
        }
        if (ImGui::SliderFloat("offset", &offset, -1.0f, 1.0f, "%0.3f"))
        {
            GrassShader->SetFloat("offset", offset);
        }
        ImGui::End();

        ImGui::Begin("Serialize");

        static char filename_buf[32] = "save.json";
        ImGui::InputText("File", filename_buf, 32);

        if (ImGui::Button("Serialize"))
        {
            json j = rlg.Serialize();

            j["current_room"] = { room->position.x, room->position.y };
            
            std::ofstream save_file;
            save_file.open(filename_buf);
            save_file << j.dump();
            save_file.close();
        }

        if (ImGui::Button("Deserialize"))
        {
            std::ifstream save_file;
            save_file.open(filename_buf);
            json j = json::parse(save_file);
            save_file.close();

            rlg.Destroy();
            rlg = generation::RoomLayoutGenerator(j, room_root);
            glm::ivec2 current_room = { j["current_room"][0], j["current_room"][1] };
            room = &rlg.rooms[current_room];
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        
        
#pragma endregion 
        glfwSwapBuffers(window);
    }
    
    SpellCaster::Destroy();
    GrassRendererManager::Destroy();
    PlayerStatsManager::Destroy();
    ChokeList::Destroy();
    drop::SpellDropQueue::Destroy();
    drop::DropManager::Destroy();
    audio::AudioManager::Destroy();
    HealthManager::Destroy();
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
 