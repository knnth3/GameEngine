#include "Animator.h"


using namespace Engine;
using namespace std;

Engine::Animator::Animator()
{
	m_transforms = nullptr;
	m_bPause = false;
	m_readyToChange = true;
	m_next = nullptr;
	m_currentAnimIndex = -1;
	m_nextAnimIndex = -1;
	LoadMesh(nullptr);
}

Engine::Animator::Animator(const std::shared_ptr<Mesh>& mesh)
{
	m_transforms = nullptr;
	m_bPause = false;
	m_readyToChange = true;
	m_next = nullptr;
	m_currentAnimIndex = -1;
	m_nextAnimIndex = -1;
	LoadMesh(mesh);
}

bool Engine::Animator::SetAnimation(int index)
{
	if (m_currentAnimIndex != index)
	{
		try
		{
 			m_next = &m_mesh->Animations.at(index);
			if (m_currentAnimIndex < 0)
			{
				m_animation = *m_next;
				m_time = 0.0;
				m_next = nullptr;
				m_currentAnimIndex = index;
			}
			else if (!m_waitToFinish.at(m_currentAnimIndex))
			{
				m_animation = *m_next;
				m_time = 0.0;
				m_next = nullptr;
				m_currentAnimIndex = index;
			}
			else
			{
				m_nextAnimIndex = index;
			}
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}

void Engine::Animator::LoadMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_time = 0.0;
	if (mesh)
	{
		m_mesh = mesh;
		m_parentJoint = mesh->ParentJoint;
		if (!m_transforms)
		{
			m_transforms = std::make_shared<vector<glm::mat4>>();
			m_transforms->resize(MAX_JOINTS);
		}
		if (!m_mesh->Animations.empty())
		{
			m_waitToFinish.resize(m_mesh->Animations.size(), false);
			m_time = 0.0;
			m_readyToChange = true;
			SetAnimation(0);
		}
	}
}

void Engine::Animator::Update(const double elapsed)
{
	if (m_next && m_readyToChange)
	{
		m_animation = *m_next;
		m_currentAnimIndex = m_nextAnimIndex;
		m_next = nullptr;
		m_nextAnimIndex = -1;
	}
	if (!m_bPause)
	{
		if (m_animation.GetLength())
		{
			std::map<std::string, glm::mat4> pose;
			UpdateTime(elapsed);
			GetNextPose(pose);
			ApplyPose(pose, m_parentJoint, glm::mat4());
		}
	}
}

void Engine::Animator::TogglePlay()
{
	m_bPause = !m_bPause;
}

void Engine::Animator::Reset()
{
	m_time = 0.0;
}

const AnimTransformPtr Engine::Animator::GetTransforms() const
{
	return m_transforms;
}

int Engine::Animator::GetAnimationCount() const
{
	return (int)m_mesh->Animations.size();
}

void Engine::Animator::WaitToFinish(int index, bool value)
{
	m_waitToFinish.at(index) = value;
}

void Engine::Animator::UpdateTime(const double elapsed)
{
	double animTime = m_animation.GetLength();
	m_time += elapsed;
	m_readyToChange = false;
	if (m_time >= animTime)
	{
		m_time = fmod(m_time, animTime);
		m_readyToChange = true;
	}
}

void Engine::Animator::GetNextPose(std::map<std::string, glm::mat4>& pose)
{
	std::map<std::string, JointTransform> lastPose;
	auto& current = m_animation.Get(m_time);
	current->GetAllTransforms(lastPose);
	double deltaTime = m_time - current->GetTimestamp();
	double range = current->GetNext()->GetTimestamp() - current->GetTimestamp();
	double progression = deltaTime / range;
	progression = (progression < 1.0) ? progression : 1.0;
	progression = (progression > 0.0) ? progression : 0.0;

	for (const auto& joint : lastPose)
	{
		auto& prev = current->GetTransform(joint.first);
		auto& next = current->GetNext()->GetTransform(joint.first);
		JointTransform interpolated = prev.Interpolate(next, (float)progression);
		pose.emplace(joint.first, interpolated.GetLocalTransform());
	}
}

void Engine::Animator::ApplyPose(std::map<std::string, glm::mat4>& pose, JointNode& joint, const glm::mat4& parentTransform)
{
	auto jointMatrix = pose.find(joint.GetName());
	if (jointMatrix != pose.end())
	{
		glm::mat4 currentLocalTransform = jointMatrix->second;
		glm::mat4 currentTransform = parentTransform * currentLocalTransform;
		for (auto& child : joint.Children)
		{
			ApplyPose(pose, child, currentTransform);
		}

		currentTransform = currentTransform * joint.GetInverseBindTransform();
		joint.SetAnimationTransform(currentTransform);
		m_transforms->at(joint.GetIndex()) = joint.GetAnimationTransform();
	}
}
