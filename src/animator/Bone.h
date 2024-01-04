

#pragma once

/* Container for bone data */

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <model_loading/UtilityAssimp.h>



class Bone
{
public:
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

	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	void Update(float animationTime);
	glm::mat4 GetLocalTransform() const { return m_LocalTransform; }
	const std::string& GetBoneName() const { return m_Name; }
	int GetBoneID() const { return m_ID; }



	int GetPositionIndex(float animationTime) const;

	int GetRotationIndex(float animationTime) const;

	int GetScaleIndex(float animationTime) const;


private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	glm::mat4 InterpolatePosition(float animationTime);

	glm::mat4 InterpolateRotation(float animationTime);

	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;
};
