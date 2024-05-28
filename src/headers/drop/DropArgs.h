#ifndef DROP_ARGS_H
#define DROP_ARGS_H

#include "glm/glm.hpp"
#include "../global.h"
namespace drop
{
	struct FloatDropArgs
	{
		float value_;
		FloatDropArgs(float value) : value_(value) {}
	};

	struct SpellDropArgs
	{
		SPELLS type_;
		glm::vec3 position_;

		SpellDropArgs(SPELLS type, glm::vec3 position) : type_(type), position_(position) {}
	};
}


#endif // !DROP_ARGS_H
