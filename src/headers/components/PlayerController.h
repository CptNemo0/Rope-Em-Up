#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "nlohmann/json.hpp"

#include "../GameObject.h"
#include "../input/InputObserver.h"
#include "../input/InputManager.h"
#include "../physics/PBD.h"
#include "../Timer.h"
#include "Animator.h"
#include "../../headers/audio/AudioManager.h"
#include "../../headers/Shuffler.h"


#include "../typedef.h"

namespace components
{

class PlayerController : public Component, public input::InputObserver, public std::enable_shared_from_this<PlayerController>
{
public:
    int gamepadID_;
    int speed_ = 1000;
    int pull_power_ = 2000;

    s_ptr<pbd::BasicGenerator> move_generator_;
    s_ptr<pbd::BasicGenerator> pull_generator_;
    bool is_pulling_ = false, pull_lock_ = false, move_lock_ = false, interacting_ = false;
    glm::vec3 direction_ = glm::vec3(0.0f);
    glm::vec3 pull_direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
    PlayerController(int gamepadID);

    unsigned int walk_timer_ = -1;
    float walk_timer_delay_ = 0.5f;
    bool walking_timer_lock_ = false;
    Shuffler<s_ptr<audio::AudioBuffer>> walk_sounds_;
    Shuffler<s_ptr<audio::AudioBuffer>> grass_walk_sounds;
    Shuffler<s_ptr<audio::AudioBuffer>> damage_sounds_;

    void ForceStop();

    void Start() override;
    void Update() override;
    void Destroy() override;

    void OnAction(Action action, input::State state) override;

    void PullRope();

    PlayerController(json &j);
    json Serialize() override;
};

} // namespace Components

#endif // !PLAYERCONTROLLER_H