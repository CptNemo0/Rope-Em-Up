#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <map>
#include <functional>

#include "GameObject.h"
#include "Camera.h"
#include "global.h"
#include "typedef.h"

struct Scene
{
    s_ptr<GameObject> scene_root_;
    s_ptr<GameObject> HUD_root_;
    s_ptr<GameObject> HUD_text_root_;
    s_ptr<llr::Camera> camera_;

    Scene()
    {
        scene_root_ = GameObject::Create();
        HUD_root_ = GameObject::Create();
        HUD_text_root_ = GameObject::Create();
    }

    void Halt()
    {
        scene_root_->Halt();
        HUD_root_->Halt();
        HUD_text_root_->Halt();
    }

    void Continue()
    {
        scene_root_->Continue();
        HUD_root_->Continue();
        HUD_text_root_->Continue();
        Global::i_->active_camera_ = camera_;
    }

    std::function<void(float)> OnUpdate;
    std::function<void()> OnStart;
};

class SceneManager
{
private:
    SceneManager() = default;
    ~SceneManager() = default; 
public:
    static SceneManager *i_;
    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new SceneManager();
        }
    }
    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

    std::map<string, s_ptr<Scene>> scenes_;
    s_ptr<Scene> current_scene_;

    void SwitchScene(string scene_name);
    void AddScene(string scene_name, s_ptr<Scene> scene);
};

#endif // !SCENEMANAGER_H