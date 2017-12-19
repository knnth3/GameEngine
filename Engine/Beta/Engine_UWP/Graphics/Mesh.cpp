#include "Mesh.h"

void Graphics::Mesh::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	verts.clear();
	indices.clear();

	verts.insert(verts.end(), m_vertices.begin(), m_vertices.end());
	indices.insert(indices.end(), m_indices.begin(), m_indices.end());
}

size_t Graphics::Mesh::GetVertices(std::vector<Vertex>& verts)
{
	verts.insert(verts.end(), m_vertices.begin(), m_vertices.end());
	return verts.size();
}

size_t Graphics::Mesh::GetIndices(std::vector<Index>& indices)
{
	indices.insert(indices.end(), m_indices.begin(), m_indices.end());
	return indices.size();
}

uint32_t Graphics::Mesh::GetIndexCount()
{
	if (!m_vertices.empty())
		return (uint32_t)m_indices.size();
	else
		return 0u;
}

float Graphics::Mesh::GetHeight()
{
	return m_height;
}
