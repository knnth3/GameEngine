#pragma once
#include "Vertex.h"

namespace Engine
{
	typedef int32_t MeshID;
	typedef uint32_t Index;

	class Mesh
	{
	public:
		Mesh();
		void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
		size_t GetVertices(std::vector<Vertex>& verts);
		size_t GetIndices(std::vector<Index>& indices);
		uint32_t GetIndexCount();
		float GetHeight();


		bool m_bUsingVertexColors;
		float m_height;
		std::vector<Vertex> m_vertices;
		std::vector<Index> m_indices;
	};



}
