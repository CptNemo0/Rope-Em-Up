#ifndef TUTORIAL_MANAGER_H
#define TUTORIAL_MANAGER_H

#include "generation/Room.h"
#include "generation/RoomGenerator.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include "GameObject.h"
#include <iostream>
#include "SceneManager.h"
#include "components/TextRenderer.h"
#include "res/Resources.h"

class TutorialManager
{
public:
    static TutorialManager* i_;
private:
    TutorialManager(GLFWvidmode* mode);
    ~TutorialManager() = default;

public:

    GLFWvidmode* mode_;
    generation::RoomLayoutGenerator* rlg_;
    int current_ = 0;
    int traversed_ = -1;

    std::unordered_map<int, std::shared_ptr<GameObject>> tutorial_map;
    
    void Init();

    void Update();

    static void Initialize(GLFWvidmode* mode)
    {
        if (i_ == nullptr)
        {
            i_ = new TutorialManager(mode);
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
};

#endif // !TUTORIAL_MANAGER_H
