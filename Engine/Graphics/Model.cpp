#include <typeinfo> 
#include "Model.h"


template<class T>
const uint32_t Graphics::Model::Polygon<T>::GetVertexCount()
{
	return (c_uint)m_vertices.size();
}
template<class T>
const uint32_t Graphics::Model::Polygon<T>::GetIndexCount()
{
	return (c_uint)m_indices.size();
}

void Graphics::Model::MeshData::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Graphics::Model::MeshData::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
}

float Graphics::Model::MeshData::GetHeight()
{
	return m_height;
}

Graphics::Model::MeshData2D::MeshData2D()
{
	m_polygons.emplace_back();
	m_polygons[0].m_vertices.emplace_back();
	m_polygons[0].m_indices.emplace_back(0U);
}

void Graphics::Model::MeshData2D::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices)
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Graphics::Model::MeshData2D::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
}