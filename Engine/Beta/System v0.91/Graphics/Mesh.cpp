#include "Mesh.h"

void Graphics::Mesh::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Graphics::Mesh::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
}

float Graphics::Mesh::GetHeight()
{
	return m_height;
}
