#pragma once
#include "nlohmann/json.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "../res/Resources.h"
#include "../animation/Animation.h"
#include "Component.h"
#include "../Timer.h"
#include "../typedef.h"

namespace components
{
	class Animator : public Component, public std::enable_shared_from_this<Animator>
	{
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		
		s_ptr<anim::Animation> m_CurrentAnimation;

		float m_CurrentTime;
		float m_DeltaTime;

		float m_BlendingTime;
		float m_BlendFactor;

		int priority_ = 0;

		unsigned int anim_timer_id_ = -1;
		bool animation_playing_ = false;

	public:
		std::map<std::string, s_ptr<anim::Animation>> m_Animations;

		s_ptr<anim::Animation> m_BlendingAnimation;
		Animator(s_ptr<anim::Animation> animation);
		Animator();
		~Animator() = default;

		void UpdateAnimation(float dt);

		void PlayAnimation(s_ptr<anim::Animation> pAnimation);
		void PlayAnimation(const std::string& animationName, int priority);

		void SetAnimation(const string& animationName, int priority);

		void AddAnimation(const std::string& name, s_ptr<anim::Animation> animation);
		void RemoveAnimation(const std::string& name);

		bool IsAttackAnimation(s_ptr<anim::Animation> animation);
		bool IsAnimation(const string& animationName);

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
		void Update() override {}
		void Destroy() override;

		Animator(json &j);
		json Serialize() override;
	};
}


