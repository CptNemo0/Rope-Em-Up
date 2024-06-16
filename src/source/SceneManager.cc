#include "../headers/SceneManager.h"

SceneManager *SceneManager::i_ = nullptr;

void SceneManager::SwitchScene(string scene_name)
{
    current_scene_->Halt();
    current_scene_ = scenes[scene_name];
    current_scene_->Continue();
}
