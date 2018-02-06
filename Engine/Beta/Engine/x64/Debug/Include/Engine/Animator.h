#pragma once
#include "Animation.h"
#include "Mesh.h"

#define MAX_JOINTS 60

namespace Engine
{
	typedef std::shared_ptr<std::vector<glm::mat4>> AnimTransformPtr;

	class Animator
	{
	public:
		Animator();
		Animator(const std::shared_ptr<Mesh>& mesh);
		void LoadAnimation(const Animation& anim);
		void LoadMesh(const std::shared_ptr<Mesh>& mesh);
		void Update(const double elapsed);
		void TogglePlay();
		void Reset();

		const AnimTransformPtr GetTransforms()const;

	private:
		void UpdateTime(const double elapsed);
		void GetNextPose(std::map<std::string, glm::mat4>& pose);
		void ApplyPose(std::map<std::string, glm::mat4>& pose, JointNode& joint, const glm::mat4& parentTransform);

		bool m_bPause;
		double m_time;
		JointNode m_parentJoint;
		Animation m_animation;
		std::shared_ptr<KeyFrame> m_currentKeyFrame;
		std::shared_ptr<std::vector<glm::mat4>> m_transforms;
	};
}
