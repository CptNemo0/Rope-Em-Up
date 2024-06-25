#ifndef ALTAR_H
#define ALTAR_H

#include "Component.h"
#include "BillboardRenderer.h"
#include "../input/InputObserver.h"
#include "../input/Action.h"
#include "../SceneManager.h"

namespace components
{

class Altar : public Component, public input::InputObserver, public std::enable_shared_from_this<Altar>
{
private:
    s_ptr<GameObject> player_1, player_2;
    s_ptr<components::Transform> transform_;
    s_ptr<components::BillboardRenderer> billboard_renderer_;

    bool is_near_ = false;

public:
    void Start() override;
    void Update() override;
    void Destroy() override;

    void OnAction(Action action, input::State state) override;
};

} // namespace components

#endif // ALTAR_H