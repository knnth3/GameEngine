#pragma once
#include "Polygon.h"

namespace Graphics
{
	typedef int32_t MeshID;

	class Mesh
	{
	public:
		GRAPHICS_DLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
		GRAPHICS_DLL_API size_t GetVertices(std::vector<Vertex>& verts);
		GRAPHICS_DLL_API size_t GetIndices(std::vector<Index>& indices);
		GRAPHICS_DLL_API uint32_t GetIndexCount();
		GRAPHICS_DLL_API float GetHeight();


		uint32_t objectID = 0;
		uint32_t vertOffset = 0;
		uint32_t indiciOffset = 0;
		std::vector<Polygon> m_polygons;
		float m_height = 0.0f;
	};



}
