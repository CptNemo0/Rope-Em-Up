#pragma once
#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "../animation/Animation.h"
#include "Component.h"
#include "../typedef.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace components
{
	class Animator : public Component
	{
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		std::map<std::string, s_ptr<anim::Animation>> m_Animations;
		s_ptr<anim::Animation> m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

		float m_BlendingTime;
		float m_BlendFactor;

	public:
		s_ptr<anim::Animation> m_BlendingAnimation;
		Animator(s_ptr<anim::Animation> animation);
		Animator();
		~Animator() = default;

		void UpdateAnimation(float dt);

		void PlayAnimation(s_ptr<anim::Animation> pAnimation);
		void PlayAnimation(const std::string& animationName);

		void AddAnimation(const std::string& name, s_ptr<anim::Animation> animation);
		void RemoveAnimation(const std::string& name);

		void BlendTwoAnimations(s_ptr<anim::Animation> pLayeredAnimation, float blendFactor);
		void BlendTwoAnimations(const std::string& pLayeredAnimation, float blendFactor);
		
		void CalculateBoneTransform(const anim::AssimpNodeData* node, glm::mat4 parentTransform);
		void CalculateBlendedBoneTransform(
			s_ptr<anim::Animation> pAnimationBase, const anim::AssimpNodeData* node,
			s_ptr<anim::Animation> pAnimationLayer, const anim::AssimpNodeData* nodeLayered,
			const float currentTimeBase, const float currentTimeLayered,
			const glm::mat4& parentTransform,
			const float blendFactor);

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


