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

	class Mesh
	{
	public:
		Mesh();
		void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
		size_t GetVertices(std::vector<Vertex>& verts);
		size_t GetIndices(std::vector<Index>& indices);
		uint32_t GetIndexCount();

		int m_creationFlags;
		float m_height;
		std::vector<Vertex> m_vertices;
		std::vector<Index> m_indices;
	};



}
