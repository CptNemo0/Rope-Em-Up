#include "../headers/SceneManager.h"

SceneManager *SceneManager::i_ = nullptr;

void SceneManager::SwitchScene(string scene_name)
{
    if (current_scene_)
    {
        current_scene_->Halt();
    }
    current_scene_ = scenes_[scene_name];
    current_scene_->Continue();
    if (current_scene_->OnStart)
    {
        current_scene_->OnStart();
    }
}

void SceneManager::AddScene(string scene_name, s_ptr<Scene> scene)
{
    scenes_.insert({scene_name, scene});
    scene->Halt();
}
