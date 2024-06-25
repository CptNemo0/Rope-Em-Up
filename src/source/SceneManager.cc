#include "../headers/SceneManager.h"

SceneManager *SceneManager::i_ = nullptr;

void SceneManager::SwitchScene(string scene_name)
{
    if (!scenes_.contains(scene_name))
    {
        cout << "Scene \"" << scene_name << "\", doesn't exist\n";
        return;
    }
    if (current_scene_)
    {
        if (current_scene_->OnExit)
        {
            current_scene_->OnExit();
        }
        current_scene_->Halt();
        if (current_scene_->menus_.contains(current_scene_->current_menu_))
        {
            current_scene_->menus_[current_scene_->current_menu_]->Disable();
        }
    }
    current_scene_ = scenes_[scene_name];
    current_scene_->Continue();
    if (current_scene_->OnStart)
    {
        current_scene_->OnStart();
    }
    if (current_scene_->menus_.contains(current_scene_->current_menu_))
    {
        current_scene_->menus_[current_scene_->current_menu_]->Enable();
    }
}

void SceneManager::AddScene(string scene_name, s_ptr<Scene> scene)
{
    scenes_.insert({scene_name, scene});
    scene->Halt();
}


