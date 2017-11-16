#pragma once
#include "Model.h"
#include <map>

namespace Graphics
{
	struct PerInstanceInfo
	{
		glm::vec4 color;
		glm::mat4 world;
	};

	struct BatchInfo
	{
		TextureID Texture;
		uint32_t IndexCountPerInstance;
		uint32_t InstanceCount;
		uint32_t StartIndexLocation;
		uint32_t BaseVertexLocation;
		uint32_t StartInstanceLocation;
	};

	struct Batch
	{
		BatchInfo info;
		std::vector<PerInstanceInfo> data;
	};


	class VertexManager
	{
	public:
		VertexManager(const uint16_t maxInstances);
		void AddModel(std::shared_ptr<Model> model);

		bool NewBatchInfo();
		void Reset();

		void GetVertexData(std::vector<Vertex>& verts, std::vector<Index>& indices);
		void GetBatchData(std::vector<PerInstanceInfo>& instances,
			std::vector<BatchInfo>& batchInfo);


	private:
		void CreateNewBatch(MeshID mesh, TextureID texture);

		bool m_bNewBatch;
		const uint16_t m_maxInstances;
		uint16_t m_currentSize;
		std::vector<Vertex> m_vertices;
		std::vector<Index> m_indices;
		std::map<std::pair<MeshID, TextureID>, Batch> m_BatchCache;
	};


}

