#pragma once
#include "Vertex.h"

namespace Engine
{
	typedef uint32_t Index;

	enum CreationTypes
	{
		CREATION_TYPE_NORMAL = 0,
		CREATION_TYPE_NO_UV = 1
	};

	class JointNode
	{
	public:
		JointNode();
		JointNode(int index, const std::string& name, glm::mat4 inverseBindTransform);
		int GetIndex()const;
		std::string GetName()const;
		glm::mat4 GetInverseBindTransform()const;
		glm::mat4 GetAnimationTransform()const;
		void SetAnimationTransform(glm::mat4 matrix);

		std::vector<JointNode> Children;

	private:

		int m_index;
		std::string m_name;
		glm::mat4 m_inverseBindTransform;
		glm::mat4 m_animationTransform;
	};

	struct Mesh
	{
	public:
		Mesh();

		int CreationFlags;
		float ModelHeight;
		JointNode ParentJoint;
		std::vector<Vertex> Vertices;
		std::vector<Index> Indices;
	};

}
