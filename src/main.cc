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
#include "headers/typedef.h"
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
#include "headers/Global.h"
#include "headers/Bloom.h"
#include "headers/Minimap.h"
#include "headers/components/FloorRenderer.h"
#include "headers/FloorRendererManager.h"
#include "headers/DifficultyManager.h"
#include "headers/SceneManager.h"
#include "headers/Menu.h"
#include "headers/SkullMinionManager.h"
#include "headers/rendering/LightsManager.h"

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

	const string kShadowDepthVertexShaderPath = "res/shaders/ShadowDepth.vert";
	const string kShadowDepthFragmentShaderPath = "res/shaders/ShadowDepth.frag";

    const string kSSAOVertexShaderPath = "res/shaders/SSAO.vert";
    const string kSSAOFragmentShaderPath = "res/shaders/SSAO.frag";

    const string kSSAOBlurVertexShaderPath = "res/shaders/SSAOBlur.vert";
    const string kSSAOBlurFragmentVerticalShaderPath = "res/shaders/SSAOBlurVertical.frag";
    const string kSSAOBlurFragmentHorizontalShaderPath = "res/shaders/SSAOBlurHorizontal.frag";

    const string kGrassVertexShaderPath = "res/shaders/GrassShaderVert.vert";
    const string kGrassFragmentShaderPath = "res/shaders/GrassShaderFrag.frag";

    const string kBloomThresholdShaderPath = "res/shaders/BloomThreshold.frag";
    const string kBloomBlurHorizontalShaderPath = "res/shaders/BloomBlurHorizontal.frag";
    const string kBloomBlurVerticalShaderPath = "res/shaders/BloomBlurVertical.frag";
    
    const string kHudBarShaderPath = "res/shaders/HUDBarShader.frag";

    const string kFloorVertexShaderPath = "res/shaders/FloorShader.vert";
    const string kFloorTCShaderPath = "res/shaders/FloorShader.tesc";
    const string kFloorTEShaderPath = "res/shaders/FloorShader.tese";
    const string kFloorFragmentShaderPath = "res/shaders/FloorShader.frag";

    const string kGreenTexturePath = "res/textures/green_texture.png";
    const string kRedTexturePath = "res/textures/red_texture.png";
    const string kHUDTexturePath = "res/textures/placeholder_icon.png";
    const string kHUDTexturePath2 = "res/textures/staly_elmnt.png";
    const string kTestSmokeTexturePath = "res/textures/test_smoke.png";
    const string kTrailTexturePath = "res/textures/trail.png";

    const string kHealthBarTexturePath = "res/textures/HealthBar.png";
    const string kHealthBarBorderTexturePath = "res/textures/HealthBarBorder.png";

    const string kHDREquirectangularPath = "res/cubemaps/puresky_2k.hdr";
    const string kHDRMenuCubemap = "res/cubemaps/menu.hdr";

    const string kCubeMeshPath = "res/models/cube_2.obj";
    const string kCapsuleMeshPath = "res/models/capsule.obj";
    const string kFemalePlayerMeshPath = "res/players/female/kobieta_poprawa_riga.fbx";
    const string kMalePlayerMeshPath = "res/players/male/player_M_Test_2.fbx";
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

    const string kBarellPath = "res/barell/barell.obj";

    const string kGrassPath = "res/grass/grass.obj";

    const string kFontPath = "res/fonts/CourierPrime-Regular.ttf";

    const string kBruhPath = "res/sounds/tentaclepull.wav";

    const string kEnemyAiInitPath = "res/config/EnemyAiInit.ini";
    const string kVehicleInitPath = "res/config/VehicleInit.ini";
    const string kRoomLayoutGenerationSettingsInitPath = "res/config/RoomLayoutGenerationSettingsInit.ini";
    const string kRoomGenerationSettingsInitPath = "res/config/RoomGenerationSettingsInit.ini";
    const string kPBDManagerInitSettingsPath = "res/config/PBDManagerInitSettings.ini";

    const string kTentaclPath = "res/enemy/enemy.obj";
	const string lTentaclFBXPath = "res/enemy/enemy_idles.fbx";
#pragma endregion Resources Paths
    
#pragma region CameraSettings
    const float kFov = 60.0f;
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
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    utility::InitImGUI(window);

    collisions::CollisionManager::Initialize();

    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::PLAYER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::LAMPS);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::TENTACLE, collisions::LAYERS::SKULL);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::LAMPS);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::PLAYER, collisions::LAYERS::SKULL);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::ROPE);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::LAMPS);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::ROPE, collisions::LAYERS::SKULL);

    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::CLUTTER, collisions::LAYERS::CLUTTER);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::CLUTTER, collisions::LAYERS::LAMPS);
    collisions::CollisionManager::i_->RemoveCollisionBetweenLayers(collisions::LAYERS::CLUTTER, collisions::LAYERS::SKULL);

    collisions::CollisionManager::i_->AddCollisionBetweenLayers(collisions::LAYERS::LAMPS, collisions::LAYERS::SKULL);

    pbd::PBDManager::Initialize(pbd_settings);
    ai::EnemyAIManager::Initialize(enemy_ai_init, enemy_vehicle_template);
    ParticleEmitterManager::Initialize();
    HealthManager::Initialize();
    audio::AudioManager::Initialize();
    drop::DropManager::Initialize();
    drop::SpellDropQueue::Initialize();
    ChokeList::Initialize();
    SpellCaster::Initialize();
    res::init_freetype();
    Global::Initialize();
    input::InputManager::Initialize(window);
    DifficultyManager::Initialize();
    SceneManager::Initialize();
    SkullMinionManager::Initialize();
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

    Global::i_->active_camera_ = camera;

    auto projection_matrix = glm::perspective(glm::radians(camera->get_fov()), camera->get_aspect_ratio(), camera->get_near(), camera->get_far());
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
    auto SSAOBlurVerticalShader = res::get_shader(kSSAOBlurVertexShaderPath, kSSAOBlurFragmentVerticalShaderPath);
    auto SSAOBlurHorizontalShader = res::get_shader(kSSAOBlurVertexShaderPath, kSSAOBlurFragmentHorizontalShaderPath);
    auto GrassShader = res::get_shader(kGrassVertexShaderPath, kGrassFragmentShaderPath);
    auto BloomThresholdShader = res::get_shader(kSSAOVertexShaderPath, kBloomThresholdShaderPath);
    auto BloomBlurVerticalShader = res::get_shader(kSSAOVertexShaderPath, kBloomBlurVerticalShaderPath);
    auto BloomBlurHorizontalShader = res::get_shader(kSSAOVertexShaderPath, kBloomBlurHorizontalShaderPath);
    auto HUDBarShader = res::get_shader(kHUDVertexShaderPath, kHudBarShaderPath);
    auto FloorShader = res::get_shader(kFloorVertexShaderPath, kFloorTCShaderPath, kFloorTEShaderPath, kFloorFragmentShaderPath);
	auto ShadowDepthShader = res::get_shader(kShadowDepthVertexShaderPath, kShadowDepthFragmentShaderPath);

#pragma endregion Shaders
    LightsManager::Initialize(ShadowDepthShader, LBufferPassShader);

    auto cubemap = make_shared<HDRCubemap>(kHDREquirectangularPath, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader, PrefilterShader, BRDFShader);
    auto menu_cubemap = make_shared<HDRCubemap>(kHDRMenuCubemap, BackgroundShader, EquirectangularToCubemapShader, IrradianceShader, PrefilterShader, BRDFShader);
    
    auto HUD_texture = res::get_texture(kHUDTexturePath);
    auto HUD_texture2 = res::get_texture(kHUDTexturePath2);
    auto Smoke_texture = res::get_texture(kTestSmokeTexturePath);
    auto trail_texture = res::get_texture(kTrailTexturePath);
    LBuffer lbuffer = LBuffer(mode->height, mode->width);
    GBuffer gbuffer = GBuffer(mode->height, mode->width);
    SSAOBuffer ssao_buffer = SSAOBuffer(mode->height, mode->width, SSAOPrecision::HIGH_SSAO);
    SSAOBlurBuffer ssao_blur_buffer = SSAOBlurBuffer(mode->height, mode->width);
    Bloom bloom = Bloom(mode->height, mode->width);
    ppc::Postprocessor postprocessor = ppc::Postprocessor(mode->width, mode->height, PostprocessingShader);
    FloorRendererManager::Initialize(FloorShader);
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

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        LightsManager::i_->InitCubeShadowMap(i);
		LightsManager::i_->InitPlaneShadowMap(i);
    }

#pragma endregion Lights

#pragma region Models.
    auto test_model = res::get_model(kTestPath);

    auto cube_model = res::get_model(kCubeMeshPath);
    auto capsule_model = res::get_model(kCapsuleMeshPath);
    auto F_player_model = res::get_model(kFemalePlayerMeshPath);
    auto M_player_model = res::get_model(kMalePlayerMeshPath);
    auto debug_model = res::get_model(kDebugMeshPath);
    auto enemy_model = res::get_model(kTentaclPath);
	auto enemy_fbx_model = res::get_model(lTentaclFBXPath);
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

    auto barell_model = res::get_model(kBarellPath);

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
    models.barrles.push_back(barell_model);
    

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
    
    auto game_scene_root = GameObject::Create();
    auto room_root = GameObject::Create(game_scene_root);

    generation::RoomLayoutGenerator rlg;
    std::deque<w_ptr<GameObject>> room_objects;
    std::deque<w_ptr<GameObject>> room_parts;
    std::vector<w_ptr<GameObject>> enemies_parts;

    rlg.GenerateRooms(rlgs, room_root);
    rlg.GenerateGates();

    generation::Room* room = &rlg.rooms[glm::ivec2(0, 0)];
    SkullMinionManager::i_->room_ = room;
    generation::GenerateRoom(*room, &rg_settings, &models);
    generation::BuildRoom(*room, &models, GBufferPassShader, &rlg, trail_texture, ParticleShader);
    pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
    pbd::PBDManager::i_->set_walls(walls);

    auto player_1 = GameObject::Create(game_scene_root);
    player_1->transform_->TeleportToPosition(glm::vec3(-0.5 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_1->transform_->set_scale(glm::vec3(0.01f));
    player_1->AddComponent(make_shared<components::MeshRenderer>(F_player_model, GBufferPassShader));
    player_1->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::PLAYER, gPRECISION, F_player_model, 0, player_1->transform_));
    player_1->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_1->transform_));
    player_1->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_1));
    player_1->AddComponent(HealthManager::i_->CreateHealthComponent(100.0f, PLAYER));
    player_1->AddComponent(make_shared<components::SpellSlotComponent>(components::SSC_INIT::GET_SPELL_FROM_QUEUE));
    player_1->AddComponent(make_shared<components::ParticleEmitter>(1000, trail_texture, ParticleShader, true));
    
    auto emmiter_player_1 = player_1->GetComponent<components::ParticleEmitter>();
    emmiter_player_1->emission_rate_ = 0.25f;
    emmiter_player_1->life_time_ = 0.250f;
    emmiter_player_1->start_position_displacement_ = 1.0f;
    emmiter_player_1->start_acceleration_ = glm::vec3(0.0, 10.0, 0.0);
    emmiter_player_1->start_size_ = glm::vec2(0.5f, 1.0f);
    emmiter_player_1->end_size_ = glm::vec2(1.0f, 1.25f);

    auto player_2 = GameObject::Create(game_scene_root);
    player_2->transform_->TeleportToPosition(glm::vec3(-0.7 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
    player_2->transform_->set_scale(glm::vec3(0.01f));
    player_2->AddComponent(make_shared<components::MeshRenderer>(M_player_model, GBufferPassShader));
    player_2->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::PLAYER, gPRECISION, M_player_model, 0, player_2->transform_));
    player_2->AddComponent(pbd::PBDManager::i_->CreateParticle(2.0f, 0.9f, player_2->transform_));
    player_2->AddComponent(make_shared<components::PlayerController>(GLFW_JOYSTICK_2));
    player_2->AddComponent(HealthManager::i_->CreateHealthComponent(100.0f, PLAYER));
    player_2->AddComponent(make_shared<components::SpellSlotComponent>(components::SSC_INIT::NO_SPELL));
    
    player_2->AddComponent(make_shared<components::ParticleEmitter>(1000, trail_texture, ParticleShader, true));
    auto emmiter_player_2 = player_2->GetComponent<components::ParticleEmitter>();
    emmiter_player_2->emission_rate_ = 0.25f;
    emmiter_player_2->life_time_ = 0.250f;
    emmiter_player_2->start_position_displacement_ = 1.0f;
    emmiter_player_2->start_acceleration_ = glm::vec3(0.0, 10.0, 0.0);
    emmiter_player_2->start_size_ = glm::vec2(0.5f, 1.0f);
    emmiter_player_2->end_size_ = glm::vec2(1.0f, 1.25f);

    std::vector<std::shared_ptr<GameObject>> players_vector {player_1, player_2};

    ////testing enemy fbx
	auto enemy_fbx = GameObject::Create(game_scene_root);
    enemy_fbx->transform_->set_scale(glm::vec3(2.0f));
	enemy_fbx->transform_->TeleportToPosition(glm::vec3(-0.7 * generation::kModuleSize, 0.0f, -1.0 * generation::kModuleSize));
	enemy_fbx->transform_->add_position(glm::vec3(2.0f, 0.0f, 2.0f));
    // enemy_fbx->AddComponent(make_shared<components::MeshRenderer>(enemy_fbx_model, GBufferPassShader));

#pragma region Animations
	auto enemy_anim = res::get_animation(lTentaclFBXPath, 0, enemy_fbx_model->path_);
	enemy_fbx->AddComponent(make_shared<components::Animator>());
	enemy_fbx->GetComponent<components::Animator>()->AddAnimation("Idle", enemy_anim);
	enemy_fbx->GetComponent<components::Animator>()->PlayAnimation("Idle", 1, 1.0f);

    auto F_anim_gethit = res::get_animation(kFemalePlayerMeshPath, 0, F_player_model->path_);
    auto F_anim_getkilled = res::get_animation(kFemalePlayerMeshPath, 1, F_player_model->path_);
	auto F_anim_idle = res::get_animation(kFemalePlayerMeshPath, 2, F_player_model->path_);
    auto F_anim_pull = res::get_animation(kFemalePlayerMeshPath, 3, F_player_model->path_);
	auto F_anim_upgrade = res::get_animation(kFemalePlayerMeshPath, 4, F_player_model->path_);
	auto F_anim_run = res::get_animation(kFemalePlayerMeshPath, 5, F_player_model->path_);

    player_1->AddComponent(make_shared<components::Animator>());
    player_1->GetComponent<components::Animator>()->AddAnimation("Damage", F_anim_gethit);
	player_1->GetComponent<components::Animator>()->AddAnimation("Death", F_anim_getkilled); 
	player_1->GetComponent<components::Animator>()->AddAnimation("Idle", F_anim_idle);
	player_1->GetComponent<components::Animator>()->AddAnimation("Pull", F_anim_pull);
	player_1->GetComponent<components::Animator>()->AddAnimation("Upgrade", F_anim_upgrade);
	player_1->GetComponent<components::Animator>()->AddAnimation("Run", F_anim_run);


    auto M_anim_gethit = res::get_animation(kMalePlayerMeshPath, 0, M_player_model->path_);
    auto M_anim_getkilled = res::get_animation(kMalePlayerMeshPath, 1, M_player_model->path_);
	auto M_anim_idle = res::get_animation(kMalePlayerMeshPath, 2, M_player_model->path_);
    auto M_anim_pull = res::get_animation(kMalePlayerMeshPath, 3, M_player_model->path_);
	auto M_anim_upgrade = res::get_animation(kMalePlayerMeshPath, 4, M_player_model->path_);
	auto M_anim_run = res::get_animation(kMalePlayerMeshPath, 5, M_player_model->path_);

    player_2->AddComponent(make_shared<components::Animator>());
    player_2->GetComponent<components::Animator>()->AddAnimation("Damage", M_anim_gethit);
    player_2->GetComponent<components::Animator>()->AddAnimation("Death", M_anim_getkilled);
	player_2->GetComponent<components::Animator>()->AddAnimation("Idle", M_anim_idle);
	player_2->GetComponent<components::Animator>()->AddAnimation("Pull", M_anim_pull);
	player_2->GetComponent<components::Animator>()->AddAnimation("Upgrade", M_anim_upgrade);
	player_2->GetComponent<components::Animator>()->AddAnimation("Run", M_anim_run);

    //player_2->GetComponent<components::Animator>()->m_BlendingAnimation = M_anim_getkilled;
#pragma endregion Animations

    Rope rope = Rope
    (
        player_1->transform_->get_position(),
        player_2->transform_->get_position(),
        0.125f,
        0.97f,
        game_scene_root,
        test_ball_model,
        GBufferPassShader
    );

    rope.AssignPlayerBegin(player_1);
    rope.AssignPlayerEnd(player_2);

    for (int i = 0; i < 20; i++)
    {
        rope.AddSegment(game_scene_root);
    }

    ai::EnemyAIManager::SetPlayers(player_1, player_2);
    PlayerStatsManager::Initialize(&rope, player_1, player_2);


#pragma region Camera
    auto camera_root = GameObject::Create();

    auto isometricCamera = GameObject::Create(camera_root);
    isometricCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    auto isometricCameraComp = make_shared<components::CameraComponent>(player_1, player_2, camera);
    isometricCamera->AddComponent(isometricCameraComp);
    isometricCameraComp->distanceX_ = -10.0f;
    isometricCameraComp->distanceZ_ = -10.0f;
    isometricCameraComp->height_ = 15.0f;


    auto topDownCamera = GameObject::Create(camera_root);
    topDownCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    topDownCamera->AddComponent(make_shared<components::CameraComponent>(player_1, player_2, topCamera));

    auto DebugCamera = GameObject::Create(camera_root);
    DebugCamera->transform_->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    DebugCamera->AddComponent(make_shared<components::CameraComponent>(debugCamera));

    auto menuCamera = make_shared<llr::Camera>();
    menuCamera->set_fov(kFov);
    menuCamera->set_near(kNear);
    menuCamera->set_far(kFar);
    menuCamera->set_aspect_ratio(((float)mode->width / (float)mode->height));
    menuCamera->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    menuCamera->set_pitch(0.0f);
    menuCamera->set_yaw(180.0f);

    auto MenuCamera = GameObject::Create(camera_root);
    MenuCamera->AddComponent(make_shared<components::CameraComponent>(menuCamera));

    auto isometricCameraComponent = isometricCamera->GetComponent<components::CameraComponent>();
    auto topDownCameraComponent = topDownCamera->GetComponent<components::CameraComponent>();
    auto DebugCameraComponent = DebugCamera->GetComponent<components::CameraComponent>();

#pragma endregion Camera

    auto game_HUD_root = GameObject::Create();

    auto HUD_object = GameObject::Create(game_HUD_root);
    HUD_object->AddComponent(make_shared<components::HUDRenderer>(HUD_texture, HUDshader, glm::vec4(1.0f, 1.0f, 1.0f, 0.1f)));
    HUD_object->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object->transform_->set_position(glm::vec3(-0.75f, -0.75f, 0.0f));

    auto HUD_object2 = GameObject::Create(game_HUD_root);
    HUD_object2->AddComponent(make_shared<components::HUDRenderer>(HUD_texture2, HUDshader));
    HUD_object2->transform_->set_scale(glm::vec3(0.25f, 0.25f, 1.0f));
    HUD_object2->transform_->set_position(glm::vec3(0.75f, -0.75f, 0.0f));
    HUD_object2->transform_->scale_in({-1.0f, 0.0f, 0.0f}, 1.0f / Global::i_->active_camera_->get_aspect_ratio());


    auto minimap_object = GameObject::Create(game_HUD_root);
    minimap_object->AddComponent(make_shared<components::HUDRenderer>(res::get_texture("res/textures/color.png"), HUDshader, glm::vec4(0.0f, 0.0f, 0.0f, 0.5f)));
    minimap_object->transform_->scale_in({-1.0f, -1.0f, 0.0f}, 0.4f);
    minimap_object->transform_->scale_in({-1.0f, 0.0f, 0.0f}, 1.0f / Global::i_->active_camera_->get_aspect_ratio());
    auto minimap_layer = GameObject::Create(minimap_object);
    minimap_layer->transform_->set_scale(glm::vec3(glm::sqrt(0.5f), glm::sqrt(0.5f), 1.0f));
    minimap_layer->transform_->set_rotation(glm::vec3(0.0f, 0.0f, -45.0f));
    auto minimap = Minimap(minimap_layer);

    auto player_1_hp_bar_border = GameObject::Create();
    player_1_hp_bar_border->AddComponent(make_shared<components::HUDRenderer>(res::get_texture(kHealthBarBorderTexturePath), HUDshader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    player_1_hp_bar_border->transform_->set_scale({0.25f, 0.05f , 0.0f });
    player_1_hp_bar_border->transform_->set_position({ -0.75f, -0.95f, 0.0 });

    auto player_1_hp_bar = GameObject::Create(player_1_hp_bar_border);
    player_1_hp_bar->AddComponent(make_shared<components::HUDRenderer>(res::get_texture(kHealthBarTexturePath), HUDBarShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    player_1_hp_bar->transform_->set_scale({ 0.98f, 0.96f, 0.0f});
    player_1_hp_bar->transform_->set_position({ 0.0f, 0.0f, 0.0 });


    auto player_2_hp_bar_border = GameObject::Create();
    player_2_hp_bar_border->AddComponent(make_shared<components::HUDRenderer>(res::get_texture(kHealthBarBorderTexturePath), HUDshader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    player_2_hp_bar_border->transform_->set_scale({ 0.25f, 0.05f , 0.0f });
    player_2_hp_bar_border->transform_->set_position({ 0.75f, -0.95f, 0.0 });

    auto player_2_hp_bar = GameObject::Create(player_2_hp_bar_border);
    player_2_hp_bar->AddComponent(make_shared<components::HUDRenderer>(res::get_texture(kHealthBarTexturePath), HUDBarShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    player_2_hp_bar->transform_->set_scale({ 0.98f, 0.96f, 0.0f });
    player_2_hp_bar->transform_->set_rotation({ 0.0f, 0.0, 180.0f });
    player_2_hp_bar->transform_->set_position({ 0.0f, 0.0f, 0.0 });

    auto game_HUD_text_root = GameObject::Create();

    auto maturasc_font = res::get_font(kFontPath);

    auto HUDText_object = GameObject::Create(game_HUD_text_root);
    HUDText_object->AddComponent(make_shared<components::TextRenderer>(HUDTextShader, maturasc_font, "TEST", glm::vec3(1.0f)));
    HUDText_object->transform_->set_scale(glm::vec3(1.0f, 1.0f, 1.0f));
    HUDText_object->transform_->set_position(glm::vec3(50.0f, 900.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    LBufferPassShader->Use();
    LBufferPassShader->SetInt("irradianceMap", 5);
    LBufferPassShader->SetInt("prefilterMap", 6);
    LBufferPassShader->SetInt("brdfLUT", 7);

    BackgroundShader->Use();
    BackgroundShader->SetInt("environmentMap", 0);


    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    ParticleShader->Use();
    ParticleShader->SetMatrix4("projection_matrix", projection_matrix);

    SSAOShader->Use();
    SSAOShader->SetMatrix4("projection_matrix", projection_matrix);
    SSAOShader->SetInt("height", mode->height);
    SSAOShader->SetInt("width", mode->width);
    SSAOShader->SetInt("quality",(int)ssao_buffer.quality_);
    SSAOShader->SetFloat("radius", 0.4);
    SSAOShader->SetFloat("bias", 0.1);
    ssao_buffer.SetKernel(SSAOShader);
    
    FloorShader->Use();
    FloorShader->SetMatrix4("projection_matrix", projection_matrix);

    cubemap->LoadHDRimg(window, nullptr);
    menu_cubemap->LoadHDRimg(window, nullptr);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    /*int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);*/

    float fixed_update_rate = pbd::kMsPerUpdate;
    Timer::Timer fixed_update_timer = Timer::CreateTimer(1.0f / 120.0f, [&fixed_update_timer, &fixed_update_rate, &player_1, &player_2, &camera_root, &menuCamera]()
    {
        menuCamera->yaw_ += 0.05f;
        menuCamera->pitch_ = 10.0f * glm::sin(menuCamera->yaw_ * 0.1f);
        camera_root->PropagateUpdate();
        pbd::PBDManager::i_->GeneratorUpdate();
        pbd::PBDManager::i_->Integration(fixed_update_rate);
        pbd::PBDManager::i_->ClearContacts();
        collisions::CollisionManager::i_->PredictColliders();
        collisions::CollisionManager::i_->CollisionCheckPBD(pbd::PBDManager::i_->contacts_);
        pbd::PBDManager::i_->ProjectConstraints(fixed_update_rate);
        pbd::PBDManager::i_->UpdatePositions(fixed_update_rate);
        pbd::PBDManager::i_->UpdateRotations(fixed_update_rate);
        ParticleEmitterManager::i_->Update(fixed_update_rate);

    }, nullptr, true);

    Timer::Timer slow_fixed_update_timer = Timer::CreateTimer(1.0f / 4.0f, [&rlg, &room, &minimap]
    {
        ai::EnemyAIManager::i_->UpdateAI();
        minimap.Update(rlg, room);
        SkullMinionManager::i_->UpdateMinions();
    }, nullptr, true);

#pragma region Scenes

    // Main game scene

    auto game_scene = make_shared<Scene>();
    game_scene->scene_root_ = game_scene_root;
    game_scene->HUD_root_ = game_HUD_root;
    game_scene->HUD_text_root_ = game_HUD_text_root;
    game_scene->camera_ = camera;

    SceneManager::i_->AddScene("game", game_scene);

    // Menu scene

    auto menu_HUD_root = GameObject::Create();

    auto menu = make_shared<Menu>();
    input::InputManager::i_->AddObserver(0, menu);

    auto button1 = GameObject::Create(menu_HUD_root);
    button1->transform_->scale({1.0f / Global::i_->active_camera_->get_aspect_ratio(), 1.0f, 1.0f});
    button1->transform_->scale_in({0.0f, -1.0f, 0.0f}, 0.5f);
    button1->transform_->scale({0.5f, 1.0f, 1.0f});
    button1->transform_->scale(glm::vec3(0.8f));
    button1->AddComponent(make_shared<components::HUDRenderer>(HUD_texture, HUDshader));
    menu->layout_[{0, 0}] = make_shared<MenuItem>(button1, []()
    {
        SceneManager::i_->SwitchScene("game");
    });

    auto button2 = GameObject::Create(menu_HUD_root);
    button2->transform_->scale({1.0f / Global::i_->active_camera_->get_aspect_ratio(), 1.0f, 1.0f});
    button2->transform_->scale_in({0.0f, 1.0f, 0.0f}, 0.5f);
    button2->transform_->scale({0.5f, 1.0f, 1.0f});
    button2->transform_->scale(glm::vec3(0.8f));
    button2->AddComponent(make_shared<components::HUDRenderer>(HUD_texture2, HUDshader));
    menu->layout_[{0, 1}] = make_shared<MenuItem>(button2, [&button2]()
    {
        button2->transform_->add_rotation(glm::vec3(0.0f, 0.0f, 10.0f));
    });

    menu->UpdateSelection();
    auto menu_scene = make_shared<Scene>();
    menu_scene->HUD_root_ = menu_HUD_root;
    menu_scene->camera_ = menuCamera;

    SceneManager::i_->AddScene("menu", menu_scene);

    SceneManager::i_->SwitchScene("menu");

#pragma endregion

    /////////////////////////////////////////////
    /////////////////////////////////////////////
    // GAME LOOP GAME LOOP GAME LOOP GAME LOOP //
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    
    int frame = 0;
    float frame_time = 0;
    float fps = 0;
    string debug_info;
    bool use_ssao = true;

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
        player_2->GetComponent<components::Animator>()->SetDeltaTime(delta_time);
		enemy_fbx->GetComponent<components::Animator>()->SetDeltaTime(delta_time);
        utility::DebugCameraMovement(window, DebugCameraComponent->camera_, delta_time);
        input::InputManager::i_->Update();
        audio::AudioManager::i_->Update();

#pragma region Rooms

        static bool moving_through_room = false;

        bool no_enemies = room->enemies->transform_->children_.empty();

        if (no_enemies)
        {
            for (auto& gate : room->gates->transform_->children_)
            {
                gate->game_object_->GetComponent<components::ParticleEmitter>()->emit_particles_ = true;
            }
        }

        if (!moving_through_room && no_enemies)
        {
            glm::ivec2 current_room_pos = room->position;
            glm::ivec2 move_direction = generation::GetMoveDirection(room, player_1, player_2);
            glm::ivec2 next_room_pos = current_room_pos + move_direction;

            if ((current_room_pos != next_room_pos) && rlg.rooms.contains(next_room_pos))
            {
                DifficultyManager::i_->UpdateHealth(player_1, player_2);
                DifficultyManager::i_->UpdateSettings(&rg_settings);
                //cout << "GOING THROUGH ROOM";
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

                Timer::AddTimer(postprocessor.transition_vignette_time_, [&room, &rlg, &rg_settings, &models, &next_room_pos, &GBufferPassShader, &move_direction, &player_1, &player_2, &rope, &pc1, &pc2, &postprocessor, &trail_texture, &ParticleShader]()
                {
                    generation::ChangeRooms(room, rlg, rg_settings, models, next_room_pos, trail_texture, GBufferPassShader, ParticleShader);
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


                DifficultyManager::i_->UpdateRoom(room);

                for (auto& minion : SkullMinionManager::i_->minions_)
                {
                    minion->health_component_->health_ = -1.0f;
                }

            }
        }
       
#pragma endregion

#pragma region Collisions and Physics
  
        rope.ChokeCheck(room);
        Timer::UpdateTimer(fixed_update_timer, delta_time);
        Timer::UpdateTimer(slow_fixed_update_timer, delta_time);
        HealthManager::i_->DeathUpdate();
        
        // glfwGetKey(window, GLFW_KEY_SPACE
        // HealthManager::i_->something_died_
        HUDBarShader->Use();
        HUDBarShader->SetFloat("percentage", std::cosf(glfwGetTime() ) * 0.5f + 0.5f);

        static float cast_time = 2.0f;
        static float slowdown_smooth_factor = 0.055f;
        if (HealthManager::i_->something_died_ && HealthManager::i_->what_ == MONSTER)
        {
            //cout << HealthManager::i_->what_ << " " << HealthManager::i_->where_.x << " " << HealthManager::i_->where_.z << endl;

            auto ss1 = player_1->GetComponent<components::SpellSlotComponent>()->type_;
            auto ss2 = player_2->GetComponent<components::SpellSlotComponent>()->type_;

            if (!(ss1 == NOT_A_SPELL && ss2 == NOT_A_SPELL))
            {
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
        }

        drop::DropManager::i_->DropHp(players_vector);
        drop::DropManager::i_->DropSpells(*room);
        drop::DropManager::i_->DropExp();

#pragma endregion

#pragma region GO Update and Draw
       
        glViewport(0, 0, mode->width, mode->height);

        auto active_camera = Global::i_->active_camera_;
        
        // Bind buffer - Use Shader - Draw 
        gbuffer.Bind();

        projection_matrix = glm::perspective(glm::radians(active_camera->get_fov()), active_camera->get_aspect_ratio(), active_camera->get_near(), active_camera->get_far());

        BackgroundShader->Use();
        BackgroundShader->SetMatrix4("view_matrix", active_camera->GetViewMatrix());
        BackgroundShader->SetMatrix4("projection_matrix", projection_matrix);

        if (SceneManager::i_->current_scene_ == SceneManager::i_->scenes_["game"])
        {
            cubemap->BindEnvCubemap(BackgroundShader);
            cubemap->RenderCube();
        }
        if (SceneManager::i_->current_scene_ == SceneManager::i_->scenes_["menu"])
        {
            menu_cubemap->BindEnvCubemap(BackgroundShader);
            menu_cubemap->RenderCube();
        }
       
        GrassShader->Use();
        GrassShader->SetMatrix4("view_matrix", active_camera->GetViewMatrix());
        GrassShader->SetMatrix4("projection_matrix", projection_matrix);
        GrassShader->SetFloat("time", glfwGetTime());
        GrassShader->SetVec3("pp1", player_1->transform_->get_position());
        GrassShader->SetVec3("pp2", player_2->transform_->get_position());
        GrassRendererManager::i_->Draw(GrassShader->get_id());

        FloorRendererManager::i_->Draw();

        GBufferPassShader->Use();
        GBufferPassShader->SetMatrix4("view_matrix", active_camera->GetViewMatrix());

        GBufferPassShader->SetMatrix4("projection_matrix", projection_matrix);
        GBufferPassShader->SetInt("numBones", MAX_BONES);

        if (SceneManager::i_->current_scene_->scene_root_)
            SceneManager::i_->current_scene_->scene_root_->PropagateUpdate();

        //////////////////////////////////
        // Bind buffer - Bind textures - Use Shader - Draw 

        if (use_ssao)
        {
            glViewport(0, 0, float(mode->width) * 0.5f, float(mode->height) * 0.5f);
            ssao_buffer.Bind();
            SSAOShader->Use();
            ssao_buffer.BindTextures(SSAOShader, gbuffer.view_position_texture_, gbuffer.normal_texture_, gbuffer.mask_texture_);
            ssao_buffer.Draw();

            ssao_blur_buffer.Bind();

            SSAOBlurVerticalShader->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ssao_buffer.ssao_texture_);
            SSAOBlurVerticalShader->SetInt("ssao_texture", 0);
            ssao_blur_buffer.Draw();

            SSAOBlurHorizontalShader->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ssao_blur_buffer.intermediate_texture_);
            SSAOBlurHorizontalShader->SetInt("intermediate_texture", 0);
            ssao_blur_buffer.Draw();

            glViewport(0, 0, mode->width, mode->height);
        }
        //////////////////////////////////
        
        
        // Bind buffer - Bind textures - Use Shader - Draw 
        lbuffer.Bind();
        LBufferPassShader->Use();
        LBufferPassShader->SetBool("use_ssao", use_ssao);
        gbuffer.BindTextures(LBufferPassShader);

        if (SceneManager::i_->current_scene_ == SceneManager::i_->scenes_["game"])
        {
            cubemap->BindIrradianceMap(LBufferPassShader);
            cubemap->BindIBLmaps(LBufferPassShader);
        }
        if (SceneManager::i_->current_scene_ == SceneManager::i_->scenes_["menu"])
        {
            menu_cubemap->BindIrradianceMap(LBufferPassShader);
            menu_cubemap->BindIBLmaps(LBufferPassShader);
        }

        glActiveTexture(GL_TEXTURE4);
        LBufferPassShader->SetInt("ssao_texture", 4);
        glBindTexture(GL_TEXTURE_2D, ssao_blur_buffer.intermediate_texture_);

        LBufferPassShader->SetVec3("camera_position", active_camera->get_position());

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
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].intensity", point_light_intensity + 0.6f * std::sinf(glfwGetTime() * 0.75f));
            LBufferPassShader->SetFloat("pointLight[" + std::to_string(i) + "].intensity", point_light_intensity);
			//LightsManager::i_->DepthToTexture(glm::vec3(room->lamp_positions[i].x, lamp_h, room->lamp_positions[i].z), i, true);
            //LightsManager::i_->BindCubeShadowMap(LBufferPassShader, i);
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

		
		/*LightsManager::i_->BindPlaneShadowMap(LBufferPassShader, 0);
        LightsManager::i_->BindPlaneShadowMap(LBufferPassShader, 1);
        LightsManager::i_->DepthToTexture(glm::vec3(room->lamp_positions[0].x, lamp_h, room->lamp_positions[0].z), 0);
		LightsManager::i_->DepthToTexture(glm::vec3(room->lamp_positions[1].x, lamp_h, room->lamp_positions[1].z), 1);
		for (int i = 0; i < room->lamp_positions.size(); i++)
		{
			LightsManager::i_->RenderFromLightPov(i);
		}
        LightsManager::i_->RenderFromLightPov(16);
        LightsManager::i_->RenderFromLightPov(16 + 1);*/



        // LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS - LIGHTS
        lbuffer.Draw();
        
        glViewport(0, 0, mode->width, mode->height);
        // BLOOM
        if (lbuffer.bloom_)
        {
           glViewport(0, 0, 0.5 * mode->width, 0.5 * mode->height);

            bloom.Bind();

            BloomThresholdShader->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lbuffer.color_texture_);
            BloomThresholdShader->SetInt("color_texture", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gbuffer.emissive_texture_);
            BloomThresholdShader->SetInt("emission_texture", 1);
            lbuffer.Draw();

            glViewport(0, 0, mode->width, mode->height);
        }
        
        
        //////////////////////////////////

        // FORWARD PASS - FORWARD PASS - FORWARD PASS - FORWARD PASS
    
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.g_buffer_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lbuffer.l_buffer_);
        glBlitFramebuffer(0, 0, mode->width, mode->height, 0, 0, mode->width, mode->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        ParticleShader->Use();
        ParticleShader->SetMatrix4("view_matrix", active_camera->GetViewMatrix());

        ParticleEmitterManager::i_->Draw();

        glDisable(GL_BLEND);

        
        // Bind buffer - Bind textures - Use Shader - Draw 
        postprocessor.Bind();
        PostprocessingShader->Use();
        lbuffer.BindTextures(PostprocessingShader);
        PostprocessingShader->SetFloat("if_time", glfwGetTime());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bloom.downscaled_);
        PostprocessingShader->SetInt("bloom_texture", 1);
        postprocessor.Draw();
        //////////////////////////////////

#pragma endregion

#pragma region Interface

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        HUDshader->Use();
        if (SceneManager::i_->current_scene_->HUD_root_)
            SceneManager::i_->current_scene_->HUD_root_->PropagateUpdate();
        
        HUDTextShader->Use();
        HUDTextShader->SetMatrix4("projection_matrix", ortho_matrix);

        HUDText_object->GetComponent<components::TextRenderer>()->text_ = debug_info;
        if (SceneManager::i_->current_scene_->HUD_text_root_)
            SceneManager::i_->current_scene_->HUD_text_root_->PropagateUpdate();

        float p1p = 0.0;
        float p2p = 0.0;
        if (player_1 != nullptr)
        {
            auto p1hc = player_1->GetComponent<components::HealthComponent>();
            p1p = p1hc->health_ / p1hc->max_health_;
        }

        if (p1p < 0.25f)
        {
            player_1_hp_bar->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(5.0f, 0.0, 0.0, 1.0f);
            auto scale = glm::vec3( 0.98f, 0.96f, 0.0f )+ (sinf(glfwGetTime() * (1 / p1p)) * 0.5f + 0.5f) * glm::vec3(0.0f, 1.0f, 0.0f);
            player_1_hp_bar->transform_->set_scale(scale);
        }
        else
        {
            player_1_hp_bar->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(1.0f, 1.0, 1.0, 1.0f);
            player_1_hp_bar->transform_->set_scale({ 0.98f, 0.96f, 0.0f });
        }

        HUDBarShader->Use();
        HUDBarShader->SetFloat("percentage", p1p);
        player_1_hp_bar_border->PropagateUpdate();


        if (player_2 != nullptr)
        {
            auto p2hc = player_2->GetComponent<components::HealthComponent>();
            p2p = p2hc->health_ / p2hc->max_health_;
        }

        if (p2p < 0.25f)
        {
            player_2_hp_bar->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(5.0f, 0.0, 0.0, 1.0f);
            auto scale = glm::vec3(0.98f, 0.96f, 0.0f) + (sinf(glfwGetTime() * (1 / p2p)) * 0.5f + 0.5f) * glm::vec3(0.0f, 1.0f, 0.0f);
            player_2_hp_bar->transform_->set_scale(scale);
        }
        else
        {
            player_2_hp_bar->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(1.0f, 1.0, 1.0, 1.0f);
            player_2_hp_bar->transform_->set_scale({ 0.98f, 0.96f, 0.0f });
        }

        HUDBarShader->Use();
        HUDBarShader->SetFloat("percentage", p2p);
        player_2_hp_bar_border->PropagateUpdate();
        

        frame++;
        fps += 1.0f / delta_time;
        frame_time += delta_time;

        if (frame == 30)
        {
            fps /= 30.0f;
            frame_time /= 30.0f;

            debug_info = "fps: " + std::to_string(fps) + "\nframe time: " + std::to_string(frame_time);

            frame = 0;
            fps = 0.0f;
            frame_time = 0.0f;
        }
        
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
        if (ImGui::Combo("Camera Type", &selectedItem, items, IM_ARRAYSIZE(items)))
        {
            switch (selectedItem)
            {
                case 0:
                    Global::i_->active_camera_ = isometricCameraComponent->camera_;
                    break;
                case 1:
                    Global::i_->active_camera_ = topDownCameraComponent->camera_;
                    break;
                case 2:
                    Global::i_->active_camera_ = DebugCameraComponent->camera_;
                    break;
            }
        }
        switch (selectedItem)
        {
        case 0:
            ImGui::SliderFloat("Y", &isometricCameraComponent->height_, 1.0f, 20.0f, "%.0f");
            ImGui::SliderFloat("X", &isometricCameraComponent->distanceX_, -10.0f, 10.0f, "%.0f");
            ImGui::SliderFloat("Z", &isometricCameraComponent->distanceZ_, -10.0f, 10.0f, "%.0f");
            break;
        case 1:
            ImGui::SliderFloat("Y", &topDownCameraComponent->height_, 1.0f, 100.0f, "%.2f");
            break;
        case 2:
            break;
        }
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

        
        if (ImGui::Checkbox("SSAO", &use_ssao))
        {
            LBufferPassShader->Use();
            LBufferPassShader->SetBool("use_ssao", use_ssao);
        }

        ImGui::Checkbox("Bloom", &lbuffer.bloom_);
        ImGui::End();

        ImGui::Begin("Halt test");
        static bool cb = false;
        if (ImGui::Checkbox("Halted", &cb))
        {
            if (cb)
            {
                SceneManager::i_->current_scene_->scene_root_->Halt();
                SceneManager::i_->current_scene_->HUD_root_->Halt();
                SceneManager::i_->current_scene_->HUD_text_root_->Halt();
            }
            else
            {
                SceneManager::i_->current_scene_->scene_root_->Continue();
                SceneManager::i_->current_scene_->HUD_root_->Continue();
                SceneManager::i_->current_scene_->HUD_text_root_->Continue();
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

        ImGui::Begin("Serialize");

        static char filename_buf[32] = "save.json";
        ImGui::InputText("File", filename_buf, 32);

        if (ImGui::Button("Serialize"))
        {
            json j = rlg.Serialize();
            j["rope"] = rope.Serialize();
            j["player_1"] = player_1->Serialize();
            j["player_2"] = player_2->Serialize();

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
            pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
            pbd::PBDManager::i_->set_walls(walls);
            rg_settings.generated_rooms = rlg.built_rooms_;
            minimap.Rebuild(rlg);

            rope.Deserialize(j["rope"]);
            player_1->Destroy();
            player_2->Destroy();
            *player_1 = *GameObject::Deserialize(j["player_1"]);
            *player_2 = *GameObject::Deserialize(j["player_2"]);
            game_scene_root->transform_->AddChild(player_1->transform_);
            game_scene_root->transform_->AddChild(player_2->transform_);
            rope.rope_constraints_.pop_back();
            rope.rope_constraints_.pop_front();
            rope.AssignPlayerBegin(player_1);
            rope.AssignPlayerEnd(player_2);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        
        
#pragma endregion 
        glfwSwapBuffers(window);
    }
    
    SkullMinionManager::Destroy();
    DifficultyManager::Destroy();
    FloorRendererManager::Destroy();
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
    SceneManager::Destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
 