#include "Mesh.h"

Engine::Mesh::Mesh()
{
	CreationFlags = 0;
	ModelHeight = 0.0;
}

Engine::JointNode::JointNode()
{
	m_index = -1;
	m_name = "";
	m_animationTransform = glm::mat4();
	m_inverseBindTransform = glm::mat4();
}

Engine::JointNode::JointNode(int index, const std::string & name, glm::mat4 inverseBindTransform)
{
	m_index = index;
	m_name = name;
	m_inverseBindTransform = inverseBindTransform;
	m_animationTransform = glm::mat4();
}

int Engine::JointNode::GetIndex() const
{
	return m_index;
}

std::string Engine::JointNode::GetName() const
{
	return m_name;
}

glm::mat4 Engine::JointNode::GetInverseBindTransform() const
{
	return m_inverseBindTransform;
}

glm::mat4 Engine::JointNode::GetAnimationTransform() const
{
	return m_animationTransform;
}

void Engine::JointNode::SetAnimationTransform(glm::mat4 matrix)
{
	m_animationTransform = matrix;
}