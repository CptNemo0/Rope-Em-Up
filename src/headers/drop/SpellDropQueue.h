#ifndef SPELL_DROP_QUEUE_H
#define SPELL_DROP_QUEUE_H

#include "../drop/Drop.h"
#include "../drop/DropArgs.h"
#include <queue>
#include "../typedef.h"

namespace drop
{
	class SpellDropQueue
	{
    public:
        static SpellDropQueue* i_;
    private:
        SpellDropQueue();
        ~SpellDropQueue() = default;

    public:

        static void Initialize()
        {
            if (i_ == nullptr)
            {
                i_ = new SpellDropQueue();
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

        std::queue<SPELLS> queue_;
        
        // indeksowane enumem SPELLS
        std::vector<std::shared_ptr<Model>> drop_meshes;
        std::shared_ptr<Shader> shader;

        SPELLS GetSpellFromQueue();
	};
}

#endif // !SPELL_DROP_QUEUE_H
