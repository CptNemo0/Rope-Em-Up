#ifndef DROP_MANAGER_H
#define DROP_MANAGER_H

#include <stack>
#include "Drop.h"
#include "DropArgs.h"
#include "../GameObject.h"
#include "../generation/Room.h"
#include "../components/HealthComponent.h"

namespace drop
{
	class DropManager
	{
    public:
        static DropManager* i_;
    private:
        DropManager();
        ~DropManager() = default;

    public:

        static void Initialize()
        {
            if (i_ == nullptr)
            {
                i_ = new DropManager();
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

		std::stack<ExpDrop> exp_stack_;
		std::stack<HpDrop> hp_stack_;
		std::stack<SpellDrop> spell_stack_;

		void AddHpDrop(HpDrop& drop);
		void AddExpDrop(ExpDrop& drop);
		void AddSpellDrop(SpellDrop& drop);

		void DropExp();
		void DropHp(std::vector<std::shared_ptr<GameObject>>& to_be_healed);
		void DropSpells(generation::Room& room);
	};
}


#endif // !DROP_MANAGER_H
