#include "Animator.h"


using namespace Engine;
using namespace std;

Engine::Animator::Animator()
{
	m_transforms = nullptr;
	LoadMesh(nullptr);
	m_bPause = false;
}

Engine::Animator::Animator(const std::shared_ptr<Mesh>& mesh)
{
	m_transforms = nullptr;
	LoadMesh(mesh);
	m_bPause = false;
}

void Engine::Animator::LoadAnimation(const Animation& anim)
{
	m_animation = anim;
	m_time = 0.0;
}

void Engine::Animator::LoadMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_time = 0.0;
	if (mesh)
	{
		m_parentJoint = mesh->ParentJoint;
		if (!m_transforms)
		{
			m_transforms = std::make_shared<vector<glm::mat4>>();
			m_transforms->resize(MAX_JOINTS);
		}
		if (!mesh->Animations.empty())
		{
			LoadAnimation(mesh->Animations.front());
		}
	}
}

void Engine::Animator::Update(const double elapsed)
{
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

void Engine::Animator::UpdateTime(const double elapsed)
{
	double animTime = m_animation.GetLength();
	m_time += elapsed;
	if (m_time >= animTime)
	{
		m_time = fmod(m_time, animTime);
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
