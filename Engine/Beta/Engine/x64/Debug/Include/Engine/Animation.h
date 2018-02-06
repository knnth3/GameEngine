#pragma once
#include "DllSettings.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	// In GLM the angle must be in degrees here, so convert it.
	class JointTransform
	{
	public:
		JointTransform();
		JointTransform(glm::vec3 position, glm::quat rotation);
		JointTransform Interpolate(const JointTransform & other, float progression);
		glm::mat4 GetLocalTransform();

	private:
		glm::vec3 m_position;
		glm::quat m_rotation;
	};

	class KeyFrame
	{
	public:
		KeyFrame(const std::map<std::string, JointTransform>& transforms, 
			std::shared_ptr<KeyFrame> previous, std::shared_ptr<KeyFrame> next, float timestamp);
		JointTransform GetTransform(const std::string& name)const;
		void GetAllTransforms(std::map<std::string, JointTransform>& map)const;
		const float GetTimestamp() const;
		const std::shared_ptr<KeyFrame> GetNext()const;
		const std::shared_ptr<KeyFrame> GetPrev()const;

		void SetNext(std::shared_ptr<KeyFrame>& next);
		void SetPrevious(std::shared_ptr<KeyFrame>& previous);

	private:
		const float m_timestamp;
		const std::map<std::string, JointTransform> m_transforms;
		std::shared_ptr<KeyFrame> m_previous;
		std::shared_ptr<KeyFrame> m_next;
	};

	//Assumes time in seconds
	class Animation
	{
	public:
		Animation();
		Animation(const std::vector<std::shared_ptr<KeyFrame>>& first, const double length, int numKeyFrames);
		void operator=(const Animation& anim);
		const std::shared_ptr<KeyFrame> Get(double timestamp)const;
		double GetLength()const;
		double GetInterval()const;

	private:
		double m_length;
		double m_interval;
		std::vector<std::shared_ptr<KeyFrame>> m_KeyFrames;
	};

}