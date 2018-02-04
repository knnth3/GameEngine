#include "VertexManager.h"
#include "MeshLoader.h"

Engine::VertexManager::VertexManager(const uint16_t maxInstances):
	m_maxInstances(maxInstances)
{
	m_bNewBatch = false;
}

void Engine::VertexManager::AddModel(const Model& model)
{
	int uniqueID = model.GetObjectID();

	//If batch doesn't exit, make a new one
	if (m_BatchCache.find(uniqueID) == m_BatchCache.end())
	{
		m_bNewBatch = true;
		CreateNewBatch(model);
	}

	if (m_BatchCache[uniqueID].Info.InstanceCount < m_maxInstances)
	{
		//increment instance count
		m_BatchCache[uniqueID].Info.InstanceCount++;

		//Fill out the per-instace info
		PerInstanceInfo info;
		info.color = model.GetColor();
		info.textureBounds = model.GetTextureBounds();
		info.world = glm::transpose(model.GetModelMatrix());

		//Push back info into currnet batch's vector
		m_BatchCache[uniqueID].Data.push_back(info);
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
		pair.second.Info.InstanceCount = 0;
		pair.second.Data.clear();
	}
}

void Engine::VertexManager::CreateNewBatch(const Model& model)
{
	int mesh = model.GetMesh();
	std::string texture = model.GetTexture();
	int style = model.GetDrawStyle();

	//Get Mesh data
	std::shared_ptr<Mesh> data;
	std::vector<Index> newIndices;
	MeshLoader::GrabMeshData(mesh, data);
	size_t originalVertSize = m_vertices.size();
	size_t originalIndexSize = m_indices.size();
	m_vertices.insert(m_vertices.end(), data->Vertices.begin(), data->Vertices.end());
	newIndices.insert(newIndices.end(), data->Indices.begin(), data->Indices.end());

	for (size_t i = 0; i < newIndices.size(); i++)
	{
		m_indices.push_back((Index)originalVertSize + newIndices[i]);
	}

	//Save batch info
	Batch newBatch;
	newBatch.Info.Texture = texture;
	newBatch.Info.Style = style;
	newBatch.Info.IndexCountPerInstance = (uint32_t)newIndices.size();
	newBatch.Info.InstanceCount = 0;
	newBatch.Info.StartIndexLocation = (uint32_t)originalIndexSize;

	if (data->CreationFlags & CREATION_TYPE_NO_UV)
		newBatch.Info.UsingVertexColors = true;
	else
		newBatch.Info.UsingVertexColors = false;

	//Animation Data
	newBatch.JointTransforms = model.GetTransforms();

	//Create new batch
	m_BatchCache[model.GetObjectID()] = newBatch;
}