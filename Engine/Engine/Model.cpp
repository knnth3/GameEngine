#include <typeinfo> 
#include "Model.h"
#include "Model3D.h"


template<class T>
c_uint Lime::Model::Polygon<T>::GetVertexCount()
{
	return (c_uint)m_vertices.size();
}
template<class T>
c_uint Lime::Model::Polygon<T>::GetIndexCount()
{
	return (c_uint)m_indices.size();
}

void Lime::Model::MeshData::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Lime::Model::MeshData::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
}

Lime::Model::MeshData2D::MeshData2D()
{
	m_polygons.emplace_back();
	m_polygons.emplace_back();
	m_polygons[0].m_vertices =
	{
		{ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(300.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.0f, -300.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }
	};
	m_polygons[1].m_vertices =
	{
		{ glm::vec3(300.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(300.0f, -300.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.0f, -300.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }
	};
	m_polygons[0].m_indices =
	{
		0,1,2
	};
	m_polygons[1].m_indices =
	{
		3,4,5
	};

}

void Lime::Model::MeshData2D::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices)
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Lime::Model::MeshData2D::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
}