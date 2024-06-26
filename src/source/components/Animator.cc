#include "./headers/components/Animator.h"

#include "../../headers/animation/AnimatorManager.h"

components::Animator::Animator(s_ptr<anim::Animation> animation)
	: m_CurrentAnimation(animation), m_CurrentTime(0.0f), m_BlendingTime(0.0f), m_BlendFactor(0.0f)
{
	m_FinalBoneMatrices.reserve(MAX_BONES);

    for (int i = 0; i < MAX_BONES; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

components::Animator::Animator()
	: m_CurrentTime(0.0f), m_BlendingTime(0.0f), m_BlendFactor(0.5f)
{
	m_FinalBoneMatrices.reserve(MAX_BONES);

	for (int i = 0; i < MAX_BONES; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}


void components::Animator::UpdateAnimation(float dt)
{
	if (m_CurrentAnimation && IsAttackAnimation(m_CurrentAnimation))
	{
		dt *= 4; // Speed up the delta time for attack animations
	}

	m_CurrentTime += dt;

	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		m_CurrentAnimation->model_->UpdateBoneTransforms(m_FinalBoneMatrices);
	}

	if (m_BlendingAnimation)
	{
		m_BlendingTime += dt;
		m_BlendingTime = fmod(m_BlendingTime, m_BlendingAnimation->GetDuration());
		BlendTwoAnimations(m_BlendingAnimation, m_BlendFactor * m_DeltaTime);
		m_BlendFactor += dt;
		if (m_BlendFactor > 1.0f)
		{
			m_CurrentAnimation = m_BlendingAnimation;
			m_CurrentTime = m_BlendingTime;
			m_BlendingAnimation = nullptr;
			m_BlendFactor = 0.0f;
		}
	}
	if (m_CurrentAnimation)
	{
		m_CurrentAnimation->model_->UpdateBoneTransforms(m_FinalBoneMatrices);
	}

}

void components::Animator::PlayAnimation(s_ptr<anim::Animation> animation)
{
	m_CurrentAnimation = animation;
	m_CurrentTime = 0.0f;
}

void components::Animator::PlayAnimation(const std::string& animationName, int priority, float prio_time)
{
	if (m_Animations.contains(animationName) && priority >= priority_)
	{
		m_CurrentAnimation = m_Animations[animationName];
		m_CurrentTime = 0.0f;
		/*m_BlendingAnimation = m_Animations[animationName];
		m_BlendFactor = 0.0f;*/
		priority_ = priority;

		Timer::AddTimer(prio_time, [this]()
		{
			if (this)
			{
				priority_ = 0;
			}
		});
	}
}

void components::Animator::SetAnimation(const string &animationName, int priority)
{
	if (m_Animations.contains(animationName) && m_CurrentAnimation != m_Animations[animationName] && priority >= priority_)
	{
		m_CurrentAnimation = m_Animations[animationName];
		m_CurrentTime = 0.0f;
		priority_ = priority;
	}
}

void components::Animator::AddAnimation(const std::string& name, s_ptr<anim::Animation> animation)
{
	m_Animations[name] = animation;
}

void components::Animator::RemoveAnimation(const std::string& name)
{
	if (m_Animations.find(name) != m_Animations.end())
		m_Animations.erase(name);
}

bool components::Animator::IsAttackAnimation(s_ptr<anim::Animation> animation)
{
	if (animation == m_Animations["Attack_1"] || 
		animation == m_Animations["Attack_2"] || 
		animation == m_Animations["Attack_3"] || 
		animation == m_Animations["Attack_4"] || 
		animation == m_Animations["Attack_5"])
		return true;
	else
		return false;

}

void components::Animator::BlendTwoAnimations(const std::string& pLayeredAnimation, float blendFactor)
{
	if (m_Animations.find(pLayeredAnimation) != m_Animations.end())
	{
		m_BlendingAnimation = m_Animations[pLayeredAnimation];
		BlendTwoAnimations(m_BlendingAnimation, blendFactor);
	}
}

void components::Animator::CalculateBoneTransform(const anim::AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}


void components::Animator::BlendTwoAnimations(s_ptr<anim::Animation> pLayeredAnimation, float blendFactor)
{
    // Speed multipliers to correctly transition from one animation to another
    float a = 1.0f;
    float b = m_CurrentAnimation->GetDuration() / pLayeredAnimation->GetDuration();
    const float animSpeedMultiplierUp = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

    a = pLayeredAnimation->GetDuration() / m_CurrentAnimation->GetDuration();
    b = 1.0f;
    const float animSpeedMultiplierDown = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

    // Current time of each animation, "scaled" by the above speed multiplier variables
    static float currentTimeBase = 0.0f;
    currentTimeBase += m_CurrentAnimation->GetTicksPerSecond() * this->m_DeltaTime * animSpeedMultiplierUp;
    currentTimeBase = fmod(currentTimeBase, m_CurrentAnimation->GetDuration());

    static float currentTimeLayered = 0.0f;
    currentTimeLayered += pLayeredAnimation->GetTicksPerSecond() * this->m_DeltaTime * animSpeedMultiplierDown;
    currentTimeLayered = fmod(currentTimeLayered, pLayeredAnimation->GetDuration());

    CalculateBlendedBoneTransform(m_CurrentAnimation, &m_CurrentAnimation->GetRootNode(), pLayeredAnimation, &pLayeredAnimation->GetRootNode(), currentTimeBase, currentTimeLayered, glm::mat4(1.0f), blendFactor);
}


// Recursive function that sets interpolated bone matrices in the 'm_FinalBoneMatrices' vector
void components::Animator::CalculateBlendedBoneTransform(
	s_ptr<anim::Animation> pAnimationBase, const anim::AssimpNodeData* node,
	s_ptr<anim::Animation> pAnimationLayer, const anim::AssimpNodeData* nodeLayered,
	float currentTimeBase, float currentTimeLayered,
	const glm::mat4& parentTransform,
	float blendFactor)
{
	std::string nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;
	Bone* pBone = pAnimationBase->FindBone(nodeName);
	if (pBone)
	{
		pBone->Update(currentTimeBase);
		nodeTransform = pBone->GetLocalTransform();
	}

	glm::mat4 layeredNodeTransform = nodeLayered->transformation;
	pBone = pAnimationLayer->FindBone(nodeName);
	if (pBone)
	{
		pBone->Update(currentTimeLayered);
		layeredNodeTransform = pBone->GetLocalTransform();
	}

	// Blend two matrices
	glm::vec3 scale0, scale1, translation0, translation1;
	glm::quat rotation0, rotation1;

	glm::vec3 skew;
	glm::vec4 perspective;

	// Decompose the matrices
	glm::decompose(nodeTransform, scale0, rotation0, translation0, skew, perspective);
	glm::decompose(layeredNodeTransform, scale1, rotation1, translation1, skew, perspective);

	// Perform the blending
	glm::vec3 finalScale = glm::mix(scale0, scale1, blendFactor);
	glm::quat finalRotation = glm::slerp(rotation0, rotation1, blendFactor);
	glm::vec3 finalTranslation = glm::mix(translation0, translation1, blendFactor);

	// Recompose the blended matrix
	glm::mat4 blendedMat = glm::translate(glm::mat4(1.0f), finalTranslation) * glm::mat4_cast(finalRotation) * glm::scale(glm::mat4(1.0f), finalScale);

	glm::mat4 globalTransformation = parentTransform * blendedMat;

	auto boneInfoMap = pAnimationBase->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap.at(nodeName).id;
		glm::mat4 offset = boneInfoMap.at(nodeName).offset;

		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (size_t i = 0; i < node->children.size(); ++i)
		CalculateBlendedBoneTransform(pAnimationBase, &node->children[i], pAnimationLayer, &nodeLayered->children[i], currentTimeBase, currentTimeLayered, globalTransformation, blendFactor);
}

void components::Animator::Destroy()
{
	m_Animations.clear();
	m_FinalBoneMatrices.clear();
	m_CurrentAnimation = nullptr;
	m_BlendingAnimation = nullptr;
	anim::AnimatorManager::i_->RemoveAnimatorComponent(shared_from_this());
}

components::Animator::Animator(json &j)
{
	string animation_path = j["animation_path"];
	string model_path = j["model_path"];
	int animation_number = j["animation_number"];

	auto animation = res::get_animation(animation_path, animation_number, model_path);
	this->Animator::Animator(animation);
}

json components::Animator::Serialize()
{
	json j;
	
	j["animation_path"] = m_CurrentAnimation->animation_path_;
	j["model_path"] = m_CurrentAnimation->model_->path_;
	j["animation_number"] = m_CurrentAnimation->animationNumber_;
	
	return j;
}
