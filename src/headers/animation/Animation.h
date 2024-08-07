#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Bone.h"
#include "AnimData.h"
#include <headers/Model.h>
#include <headers/typedef.h>


namespace anim {

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
public:
	Animation() = default;
	~Animation() = default;

	string animation_path_;
	s_ptr<Model> model_;
	int animationNumber_;

	Animation(const std::string& animationPath, int animationNumber, s_ptr<Model> model);
	Bone* FindBone(const std::string& name);

	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
};

}


#endif // !ANIMATION_H



