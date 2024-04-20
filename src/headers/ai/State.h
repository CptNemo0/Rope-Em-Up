#ifndef STATE_H
#define STATE_H

namespace ai
{
	class StateMachine;
	class State
	{
	public:

		virtual ~State() {}

		virtual void Enter(StateMachine*) = 0;
		virtual void Execute(StateMachine*) = 0;
		virtual void Exit(StateMachine*) = 0;
	};
}

#endif