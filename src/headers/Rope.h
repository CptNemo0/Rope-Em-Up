#ifndef ROPE_H

#include <memory>
#include "Transform.h"
#include "GameObject.h"
#include "Physics.h"

namespace rope
{
	const float kMaxDistance = 0.5f;
	const float kSpringConstant = 100.0f;
	const float kDamping = 0.01f;
	class RopeSegment;

	class RopeSegment
	{
	public:
		RopeSegment(std::shared_ptr<RopeSegment> left, std::shared_ptr<RopeSegment> right, std::shared_ptr<Components::Transform> transform)
		: left_(left), right_(right), transform_(transform) {}
		~RopeSegment() = default;
		std::shared_ptr<RopeSegment> left_;
		std::shared_ptr<RopeSegment> right_;
		std::shared_ptr<Components::Transform> transform_;
	};

	bool CheckRestraints(std::shared_ptr<RopeSegment> a, std::shared_ptr<RopeSegment> b, float t);
}

#define ROPE_H
#endif // !ROPE_H