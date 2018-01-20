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

	struct Joint
	{
		uint32_t Index;
		std::string Name;
		std::vector<const Joint*> Children;
	};

	struct Mesh
	{
	public:
		Mesh();

		int CreationFlags;
		float ModelHeight;
		Joint ParentJoint;
		std::vector<Vertex> Vertices;
		std::vector<Index> Indices;
	};

}
