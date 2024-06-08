#ifndef DROP_ARGS_H
#define DROP_ARGS_H

#include "glm/glm.hpp"
#include "../typedef.h"
#include "../Shader.h"
#include "../Model.h"
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
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Model> mesh;

		SpellDropArgs() : type_(SPELLS::SKULL_MINION), position_(glm::vec3(0.0f)) {}
		SpellDropArgs(SPELLS type, glm::vec3 position) : type_(type), position_(position) {}
	};
}


#endif // !DROP_ARGS_H
