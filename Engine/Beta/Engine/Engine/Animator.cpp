#include "Animator.h"


using namespace Engine;
using namespace std;

Engine::Animator::Animator()
{
	m_transforms = nullptr;
	LoadMesh(nullptr);
}

Engine::Animator::Animator(const std::shared_ptr<Mesh>& mesh)
{
	m_transforms = nullptr;
	LoadMesh(mesh);
}

void Engine::Animator::LoadAnimation(const std::shared_ptr<Animation>& anim)
{
	m_animation = anim;
	m_time = 0.0f;
}

void Engine::Animator::LoadMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_time = 0.0f;
	m_animation = nullptr;
	if (mesh)
	{
		m_parentJoint = mesh->ParentJoint;
		if (!m_transforms)
		{
			m_transforms = std::make_shared<vector<glm::mat4>>();
			m_transforms->resize(MAX_JOINTS);
		}
	}
}

void Engine::Animator::Update(const float elapsed)
{
	if (m_animation)
	{
		std::map<std::string, glm::mat4> pose;
		UpdateTime(elapsed);
		GetNextPose(pose);
		ApplyPose(pose, m_parentJoint, glm::mat4());
	}
}

void Engine::Animator::Reset()
{
	m_time = 0.0f;
}

const AnimTransformPtr Engine::Animator::GetTransforms() const
{
	return m_transforms;
}

void Engine::Animator::UpdateTime(const float elapsed)
{
	float animLength = m_animation->GetLength();
	m_time += elapsed;
	while (m_time > animLength)
	{
		m_time -= animLength;
	}
}

void Engine::Animator::GetNextPose(std::map<std::string, glm::mat4>& pose)
{
	std::map<std::string, JointTransform> lastPose;
	m_animation->GetCurrent()->GetAllTransforms(lastPose);
	float current = m_animation->GetCurrent()->GetTimestamp();
	float next = m_animation->GetCurrent()->GetNext()->GetTimestamp();
	float dTime = next - current;
	float cTime = m_time - current;
	float progression = cTime / dTime;

	for (const auto& joint : lastPose)
	{
		auto& current = m_animation->GetCurrent()->GetTransform(joint.first);
		auto& next = m_animation->GetCurrent()->GetNext()->GetTransform(joint.first);
		JointTransform interpolated = current.Interpolate(next, progression);
		pose.emplace(joint.first, interpolated.GetLocalTransform());
	}
}

void Engine::Animator::ApplyPose(std::map<std::string, glm::mat4>& pose, JointNode& joint, const glm::mat4& parentTransform)
{
	glm::mat4 currentLocalTransform = pose.at(joint.GetName());
	glm::mat4 currentTransform = parentTransform * currentLocalTransform;
	for (auto& child : m_parentJoint.Children)
	{
		ApplyPose(pose, child, currentTransform);
	}

	currentTransform = currentTransform * joint.GetInverseBindTransform();
	joint.SetAnimationTransform(currentTransform);
	m_transforms->at(joint.GetIndex()) = joint.GetAnimationTransform();
}
