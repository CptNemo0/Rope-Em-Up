#ifndef ROPE_H

#include <memory>
#include "../components/Transform.h"
#include "../GameObject.h"
#include "Physics.h"
#include "../components/Component.h"

namespace components
{
	class RopeSegment : public Component
	{
	public:
		RopeSegment(std::shared_ptr<RopeSegment> left, std::shared_ptr<RopeSegment> right, std::shared_ptr<components::Transform> transform)
			: left_(left), right_(right), transform_(transform){}
		~RopeSegment() = default;
		std::shared_ptr<RopeSegment> left_;
		std::shared_ptr<RopeSegment> right_;
		std::shared_ptr<components::Transform> transform_;
		std::string name_;
		bool is_puller_;
		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override {}
	};
}


namespace rope
{
	const float kMaxDistance = 0.25f;
	const float kSpringConstant = 200.0f;
	const float kDamping = 0.01f;
	const float kAdditionalPull = 0.5f;

	class Rope
	{
	public:
		std::shared_ptr<components::RopeSegment> left_end_;
		std::shared_ptr<components::RopeSegment> right_end_;

		std::shared_ptr<components::RopeSegment> left_puller_;
		std::shared_ptr<components::RopeSegment> right_puller_;

		Rope();
		~Rope() = default;

		void AddSegment(std::shared_ptr<components::RopeSegment> segment);
		bool InsertSegment(std::shared_ptr<components::RopeSegment> segment, int index);
		int Size();

		void CheckRestraints(std::shared_ptr<components::RopeSegment> a, std::shared_ptr<components::RopeSegment> b, float t);
		void EnforceRestraints(float t);
	};
}

#define ROPE_H
#endif // !ROPE_H