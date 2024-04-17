#include "../../headers/ai/StateMachine.h"

void ai::StateMachine::ChangeState(State* new_state)
{
	assert(current_state_ && new_state);

	current_state_->Exit(this);
	current_state_ = new_state;
	current_state_->Enter(this);
}

void ai::StateMachine::Step()
{
	current_state_->Execute(this);
}
