#ifndef SPELL_CASTER
#define SPELL_CASTER

#include "components/SpellSlotComponent.h"
#include <vector>
#include "typedef.h"

class SpellCaster
{
public:
    static SpellCaster* i_;
private:
    SpellCaster();
    ~SpellCaster() = default;

public:

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new SpellCaster();
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

    bool active_;
    std::vector <std::shared_ptr<components::SpellSlotComponent>> current_caster_;

    void QueueUpSpell(std::shared_ptr<components::SpellSlotComponent>);
    void Cast();

};

#endif // !SPELL_CASTER
