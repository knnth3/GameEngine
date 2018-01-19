#include "VertexManager.h"
#include "MeshLoader.h"

Engine::VertexManager::VertexManager(const uint16_t maxInstances):
	m_maxInstances(maxInstances)
{
	m_bNewBatch = false;
}

void Engine::VertexManager::AddModel(const Model& model)
{
	int mesh = model.GetMesh();
	std::string texture = model.GetTexture();
	int style = model.GetDrawStyle();

	auto tuple = std::make_tuple(mesh, texture, style);

	//If batch doesn't exit, make a new one
	if (m_BatchCache.find(tuple) == m_BatchCache.end())
	{
		m_bNewBatch = true;
		CreateNewBatch(mesh, texture, style);
	}

	else if (m_BatchCache[tuple].info.InstanceCount < m_maxInstances)
	{
		//increment instance count
		m_BatchCache[tuple].info.InstanceCount++;

		//Fill out the per-instace info
		PerInstanceInfo info;
		info.color = model.GetColor();
		info.textureBounds = model.GetTextureBounds();
		info.world = glm::transpose(model.GetModelMatrix());

		//Push back info into currnet batch's vector
		m_BatchCache[tuple].data.push_back(info);
	}
}

bool Engine::VertexManager::NewBatchInfo()
{
	bool result = m_bNewBatch;
	m_bNewBatch = false;
	return result;
}

void Engine::VertexManager::Reset()
{
	m_vertices.clear();
	m_indices.clear();
	m_BatchCache.clear();
}

void Engine::VertexManager::GetVertexData(std::vector<Vertex>& verts, std::vector<Index>& indices)
{
	verts = m_vertices;
	indices = m_indices;
}

void Engine::VertexManager::GetBatchData(std::vector<Batch>& batch)
{
	batch.clear();
	for (auto& pair : m_BatchCache)
	{
		batch.push_back(pair.second);
		pair.second.info.InstanceCount = 0;
		pair.second.data.clear();
	}
}

void Engine::VertexManager::CreateNewBatch(int mesh, const std::string& texture, int style)
{
	//Get Mesh data
	std::shared_ptr<Mesh> data;
	std::vector<Index> newIndices;
	MeshLoader::GrabMeshData(mesh, data);
	size_t originalVertSize = m_vertices.size();
	size_t originalIndexSize = m_indices.size();
	data->GetVertices(m_vertices);
	data->GetIndices(newIndices);

	for (size_t i = 0; i < newIndices.size(); i++)
	{
		m_indices.push_back((Index)originalVertSize + newIndices[i]);
	}

	//Save batch info
	Batch newBatch;
	newBatch.info.Texture = texture;
	newBatch.info.Style = style;
	newBatch.info.IndexCountPerInstance = (uint32_t)newIndices.size();
	newBatch.info.InstanceCount = 0;
	newBatch.info.StartIndexLocation = (uint32_t)originalIndexSize;

	if (data->m_creationFlags & CREATION_TYPE_NO_UV)
		newBatch.info.UsingVertexColors = true;
	else
		newBatch.info.UsingVertexColors = false;

	//Create new batch
	m_BatchCache[std::make_tuple(mesh, texture, style)] = newBatch;
}