#pragma once
#include "./headers/animation/Animation.h"
#include "./headers/global.h"
#include "./headers/components/Component.h"


namespace components
{
	class Animator : public Component
	{
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		anim::Animation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

	public:
		Animator(anim::Animation* Animation);
		~Animator() = default;

		void UpdateAnimation(float dt);

		void PlayAnimation(anim::Animation* pAnimation);

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

	};
}


