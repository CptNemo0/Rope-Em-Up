#pragma once
#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "../animation/Animation.h"
#include "Component.h"
#include "../global.h"

namespace components
{
	class Animator : public Component
	{
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		s_ptr<anim::Animation> m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

	public:
		Animator(s_ptr<anim::Animation> animation);
		~Animator() = default;

		void UpdateAnimation(float dt);

		void PlayAnimation(s_ptr<anim::Animation> pAnimation);

		void CalculateBoneTransform(const anim::AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices()
		{
			return m_FinalBoneMatrices;
		}

		void SetDeltaTime(float dt)
		{
			m_DeltaTime = dt;
		}

		void Start() override {}
		void Update() override { UpdateAnimation(m_DeltaTime); }
		void Destroy() override {}

		Animator(json &j);
		json Serialize() override;
	};
}

