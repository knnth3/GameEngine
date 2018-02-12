#include "Animation.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;

Engine::Animation::Animation():
	m_length(0.0f)
{
}

Engine::Animation::Animation(const std::vector<std::shared_ptr<KeyFrame>>& first, double length, int numKeyFrames) :
	m_length(length)
{
	m_KeyFrames = first;
	m_interval = double(length / numKeyFrames);
}

void Engine::Animation::operator=(const Animation & anim)
{
	m_KeyFrames = anim.m_KeyFrames;
	m_length = anim.GetLength();
	m_interval = anim.GetInterval();
}

const std::shared_ptr<KeyFrame> Engine::Animation::Get(double timestamp)const
{
	for (int index = 0; index < m_KeyFrames.size() - 1; index++)
	{
		if (m_KeyFrames[index]->GetTimestamp() <= timestamp && m_KeyFrames[index + 1]->GetTimestamp() >= timestamp)
		{
			return m_KeyFrames[index];
		}
	}
	return nullptr;
}

double Engine::Animation::GetLength()const
{
	return m_length;
}

double Engine::Animation::GetInterval() const
{
	return m_interval;
}

Engine::JointTransform::JointTransform()
{

}

Engine::JointTransform::JointTransform(glm::vec3 position, glm::quat rotation)
{
	m_position = position;
	m_rotation = rotation;
}

JointTransform Engine::JointTransform::Interpolate(const JointTransform & other, float progression)
{
	glm::vec3 pos = glm::mix(m_position, other.m_position, progression);
	glm::quat rot = glm::slerp(m_rotation, other.m_rotation, progression);

	return JointTransform(pos, rot);
}

glm::mat4 Engine::JointTransform::GetLocalTransform()
{
	glm::mat4 retVal;
	retVal = glm::translate(retVal, m_position);
	retVal = retVal * glm::toMat4(m_rotation);
	return retVal;
}

Engine::KeyFrame::KeyFrame(const std::map<std::string, JointTransform>& transforms, 
	std::shared_ptr<KeyFrame> previous, std::shared_ptr<KeyFrame> next, float timestamp):
	m_transforms(transforms),
	m_previous(previous),
	m_next(next),
	m_timestamp(timestamp)
{
}

JointTransform Engine::KeyFrame::GetTransform(const std::string & name) const
{
	return m_transforms.at(name);
}

void Engine::KeyFrame::GetAllTransforms(std::map<std::string, JointTransform>& map) const
{
	map = m_transforms;
}

const float Engine::KeyFrame::GetTimestamp()const
{
	return m_timestamp;
}

const std::shared_ptr<KeyFrame> Engine::KeyFrame::GetNext()const
{
	return m_next;
}

const std::shared_ptr<KeyFrame> Engine::KeyFrame::GetPrev()const
{
	return m_previous;
}

void Engine::KeyFrame::SetNext(std::shared_ptr<KeyFrame>& next)
{
	m_next = next;
}

void Engine::KeyFrame::SetPrevious(std::shared_ptr<KeyFrame>& previous)
{
	m_previous = previous;
}
