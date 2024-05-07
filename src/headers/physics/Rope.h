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
		RopeSegment(s_ptr<RopeSegment> left, s_ptr<RopeSegment> right, s_ptr<components::Transform> transform)
			: left_(left), right_(right), transform_(transform){}
		~RopeSegment() = default;
		s_ptr<RopeSegment> left_;
		s_ptr<RopeSegment> right_;
		s_ptr<components::Transform> transform_;
		string name_;
		bool is_puller_ = false;
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
		s_ptr<components::RopeSegment> left_end_;
		s_ptr<components::RopeSegment> right_end_;

		s_ptr<components::RopeSegment> left_puller_;
		s_ptr<components::RopeSegment> right_puller_;

		Rope();
		~Rope() = default;

		void AddSegment(s_ptr<components::RopeSegment> segment);
		bool InsertSegment(s_ptr<components::RopeSegment> segment, int index);
		int Size();

		void CheckRestraints(s_ptr<components::RopeSegment> a, s_ptr<components::RopeSegment> b, float t);
		void EnforceRestraints(float t);
	};
}

#define ROPE_H
#endif // !ROPE_H