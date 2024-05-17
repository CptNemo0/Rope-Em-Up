#ifndef BONE_H
#define BONE_H

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "AssimpGLMHelpers.h"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};


class Bone
{
private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

public:
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    glm::mat4 GetLocalTransform() { return m_LocalTransform; }
    std::string GetBoneName() const { return m_Name; }
    int GetBoneID() { return m_ID; }

    void Update(float animationTime);
    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

private:
    glm::vec3 InterpolatePosition(float animationTime);
	glm::quat InterpolateRotation(float animationTime);
	glm::vec3 InterpolateScaling(float animationTime);
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
};
#endif // !BONE_H
