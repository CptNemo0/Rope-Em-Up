#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include <cassert>

namespace ai
{
	class StateMachine
	{
		State* current_state_;

		void ChangeState(State* new_state);
		void Step();
	};
}

#endif // !STATE_MACHINE_H
