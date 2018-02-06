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
	int secondaryMesh = model.GetSecondaryMesh();
	std::string texture = model.GetTexture();
	int style = model.GetDrawStyle();

	//Get Mesh data
	std::shared_ptr<Mesh> data;
	std::vector<Index> newIndices;
	size_t originalVertSize = m_vertices.size();
	size_t originalIndexSize = m_indices.size();

	MeshLoader::GrabMeshData(secondaryMesh, data);
	int indexOffset = 0;
	int vertexOffset = 0;
	int testValue1 = 0;
	int testValue2 = 0;
	int testValue3 = 0;
	if (data)
	{
		for (auto& vert : data->Vertices)
		{
			if (vert.m_jointIDs[0] == 29 || vert.m_jointIDs[0] == 30)
			{
				vert.m_jointIDs[0] = 29;
			}
			if (vert.m_jointIDs[1] == 29 || vert.m_jointIDs[1] == 30)
			{
				vert.m_jointIDs[1] = 29;
			}
			vert.m_position.y += 2.0f;
		}
		m_vertices.insert(m_vertices.end(), data->Vertices.begin(), data->Vertices.end());
		newIndices.insert(newIndices.end(), data->Indices.begin(), data->Indices.end());
		for (size_t i = 0; i < newIndices.size(); i++)
		{
			m_indices.push_back((Index)originalVertSize + newIndices[i]);
		}
		vertexOffset = data->Vertices.size();
		indexOffset = data->Indices.size();
		testValue1 = data->Indices.at(newIndices.size()-1);
		testValue2 = data->Indices.at(newIndices.size()-2);
		testValue3 = data->Indices.at(newIndices.size()-3);
	}

	data = nullptr;
	MeshLoader::GrabMeshData(mesh, data);
	if (data)
	{
		m_vertices.insert(m_vertices.end(), data->Vertices.begin(), data->Vertices.end());
		newIndices.insert(newIndices.end(), data->Indices.begin(), data->Indices.end());
		for (size_t i = indexOffset; i < newIndices.size(); i++)
		{
			m_indices.push_back((Index)originalVertSize + vertexOffset + newIndices[i]);
		}
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