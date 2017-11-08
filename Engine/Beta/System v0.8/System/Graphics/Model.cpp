#include <typeinfo> 
#include "Model.h"
#include "Model3D.h"

std::vector<int> Graphics::Model::IDManager::currentIDs = std::vector<int>();

template<class T>
const unsigned int Graphics::Model::Polygon<T>::GetVertexCount()
{
	return (const unsigned int)m_vertices.size();
}
template<class T>
const unsigned int Graphics::Model::Polygon<T>::GetIndexCount()
{
	return (const unsigned int)m_indices.size();
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

int Graphics::Model::IDManager::AssignID()
{
	int newID = -1;
	for (int index = 0; index < currentIDs.size(); index++)
	{
		if (currentIDs[index] == -1)
		{
			return index;
		}
		else if (currentIDs[index] > newID)
			newID = currentIDs[index];
	}
	newID = newID + 1;
	currentIDs.push_back(newID);

	return newID;
}

void Graphics::Model::IDManager::RemoveID(int id)
{
	for (auto x : currentIDs)
	{
		if (x == id)
			x = -1;
	}
}
