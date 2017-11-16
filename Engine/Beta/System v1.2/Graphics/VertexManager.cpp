#include "VertexManager.h"
#include "MeshLoader.h"

Graphics::VertexManager::VertexManager(const uint16_t maxInstances):
	m_maxInstances(maxInstances)
{
	m_bNewBatch = false;
	m_currentSize = 0;
}

void Graphics::VertexManager::AddModel(std::shared_ptr<Model> model)
{
	MeshID mesh = model->GetMesh();
	TextureID texture = model->GetTexture();
	auto type = model->GetDrawStyle();
	if (m_currentSize < m_maxInstances)
	{
		//Only allow 3d objects to be rendered right now
		if (type == DrawStyle::TRIANGLE_3D)
		{
			auto pair = std::make_pair(mesh, texture);
			int index = mesh / 64;

			//If batch doesn't exit, make a new one
			if (m_BatchCache.find(pair) == m_BatchCache.end())
			{
				m_bNewBatch = true;
				CreateNewBatch(mesh, texture);
			}

			//increment instance count
			m_BatchCache[pair].info.InstanceCount++;

			//Fill out the per-instace info
			PerInstanceInfo info;
			info.color = model->GetColor();
			//info.texcoord = model->GetTextureBounds();
			info.world = glm::transpose(model->GetModelMatrix());

			//Push back info into currnet batch's vector
			m_BatchCache[pair].data.push_back(info);
			m_currentSize++;
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

void Graphics::VertexManager::GetBatchData(std::vector<PerInstanceInfo>& instances, std::vector<BatchInfo>& batchInfo)
{
	if(instances.size() != m_maxInstances)
		instances.resize(m_maxInstances);

	uint16_t instanceCount = 0;
	for (auto& pair : m_BatchCache)
	{
		pair.second.info.StartInstanceLocation = instanceCount;
		for (auto data : pair.second.data)
			instances[instanceCount++] = data;
		batchInfo.push_back(pair.second.info);
		pair.second.info.InstanceCount = 0;
		pair.second.data.clear();
		m_currentSize = 0;
	}
}

void Graphics::VertexManager::CreateNewBatch(MeshID mesh, TextureID texture)
{
	//Get Mesh data
	std::shared_ptr<Mesh> data;
	MeshLoader::GrabMeshData(mesh, data);
	size_t originalVertSize = m_vertices.size();
	size_t originalIndexSize = m_indices.size();
	data->GetVertices(m_vertices);
	size_t recievedIndices = data->GetIndices(m_indices);

	//Save batch info
	Batch newBatch;
	newBatch.info.Texture = texture;
	newBatch.info.IndexCountPerInstance = (uint32_t)recievedIndices;
	newBatch.info.InstanceCount = 0;
	newBatch.info.StartIndexLocation = (uint32_t)originalIndexSize;
	newBatch.info.BaseVertexLocation = (uint32_t)originalVertSize;
	newBatch.info.StartInstanceLocation = 0;

	//Create new batch
	m_BatchCache[std::make_pair(mesh, texture)] = newBatch;
}