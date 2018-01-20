#pragma once
#include "DllSettings.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct JointTransform
	{
		glm::vec3 position;
		glm::quat rotation;
	};

	struct KeyFrame
	{
		float timestamp;
		std::vector<JointTransform> transforms;
	};

	class Animation
	{
	public:
		Animation();
		Animation(std::vector<KeyFrame> keyframes);
		KeyFrame GetKeyFrame(int index);

	private:
		std::vector<KeyFrame> m_keyframes;
	};
}