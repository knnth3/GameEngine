#include "VertexManager.h"
#include "MeshLoader.h"

Graphics::VertexManager::VertexManager()
{
	m_bHasNewData = false;
}

void Graphics::VertexManager::AddModel(VertexManagerInfo& info)
{
	MeshID id = info.model->GetMesh();

	auto result = m_meshCache.find(id);
	if (result == m_meshCache.end())
	{
		m_bHasNewData = true;
		std::shared_ptr<Model::MeshData> data;
		Model::MeshLoader::GrabMeshData(id, data);

		std::vector<Model::Vertex> vertices;
		std::vector<Model::Index> indices;
		data->GetBuffers(vertices, indices);

		info.size = (int)indices.size();
		info.vertexOffset = (int)m_vertices.size();
		info.indexOffset = (int)m_indices.size();
		m_meshCache.emplace(id, info);

		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		m_indices.insert(m_indices.end(), indices.begin(), indices.end());
	}
	else
	{
		info.indexOffset = result->second.indexOffset;
		info.vertexOffset = result->second.vertexOffset;
		info.size = result->second.size;
	}
}

bool Graphics::VertexManager::HasNewData()
{
	bool result = m_bHasNewData;
	m_bHasNewData = false;
	return result;
}

void Graphics::VertexManager::Reset()
{
	m_meshCache.clear();
	m_vertices.clear();
	m_indices.clear();
}

void Graphics::VertexManager::GetBufferData(std::vector<Model::Vertex>& verts, std::vector<Model::Index>& indices)
{
	verts = m_vertices;
	indices = m_indices;
}

Graphics::VertexManagerInfo::VertexManagerInfo()
{
	vertexOffset = 0;
	indexOffset = 0;
	size = 0;
	model = nullptr;
}
