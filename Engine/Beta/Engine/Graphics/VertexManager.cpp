#include "VertexManager.h"
#include "MeshLoader.h"

Graphics::VertexManager::VertexManager(const uint16_t maxInstances):
	m_maxInstances(maxInstances)
{
	MeshLoader::Initialize();
	m_bNewBatch = false;
}

void Graphics::VertexManager::AddModel(const Model& model)
{
	MeshID mesh = model.GetMesh();
	std::string texture = model.GetTexture();
	DrawStyle drawStyle = model.GetDrawStyle();

	//Allow 3d and 2d draw styles
	if (drawStyle == TRIANGLE_3D || drawStyle == TRIANGLE_2D)
	{
		auto pair = std::make_pair(mesh, texture);

		//If batch doesn't exit, make a new one
		if (m_BatchCache.find(pair) == m_BatchCache.end())
		{
			m_bNewBatch = true;
			CreateNewBatch(mesh, texture, drawStyle);
		}
		else if (m_BatchCache[pair].info.InstanceCount < m_maxInstances)
		{
			//increment instance count
			m_BatchCache[pair].info.InstanceCount++;

			//Fill out the per-instace info
			PerInstanceInfo info;
			info.color = model.GetColor();
			info.textureBounds = model.GetTextureBounds();
			info.world = glm::transpose(model.GetModelMatrix());

			//Push back info into currnet batch's vector
			m_BatchCache[pair].data.push_back(info);
		}
	}
}

bool Graphics::VertexManager::NewBatchInfo()
{
	bool result = m_bNewBatch;
	m_bNewBatch = false;
	return result;
}

void Graphics::VertexManager::Reset()
{
	m_vertices.clear();
	m_indices.clear();
	m_BatchCache.clear();
}

void Graphics::VertexManager::GetVertexData(std::vector<Vertex>& verts, std::vector<Index>& indices)
{
	verts = m_vertices;
	indices = m_indices;
}

void Graphics::VertexManager::GetBatchData(std::vector<Batch>& batch)
{
	batch.clear();
	for (auto& pair : m_BatchCache)
	{
		batch.push_back(pair.second);
		pair.second.info.InstanceCount = 0;
		pair.second.data.clear();
	}
}

void Graphics::VertexManager::CreateNewBatch(MeshID mesh, const std::string& texture, DrawStyle style)
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
	newBatch.info.UsingVertexColors = data->m_bUsingVertexColors;

	//Create new batch
	m_BatchCache[std::make_pair(mesh, texture)] = newBatch;
}