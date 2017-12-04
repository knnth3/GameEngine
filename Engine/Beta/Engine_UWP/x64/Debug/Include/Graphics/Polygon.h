#pragma once
#include "Vertex.h"

namespace Graphics
{
	typedef uint32_t Index;
	//-Class that handles each Polygon face on a mesh
	class Polygon
	{
	public:
		GRAPHICS_DLL_API const unsigned int GetVertexCount();
		GRAPHICS_DLL_API const unsigned int GetIndexCount();
		std::vector<Index> m_indices;
		std::vector<Vertex> m_vertices;
	};
}