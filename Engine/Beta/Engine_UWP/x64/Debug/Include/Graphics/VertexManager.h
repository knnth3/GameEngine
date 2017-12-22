#pragma once
#include "Model.h"
#include <map>

namespace Graphics
{
	struct PerInstanceInfo
	{
		glm::mat4 world;
		glm::vec4 color;
		glm::vec4 textureBounds;
	};

	struct BatchInfo
	{
		TextureID Texture;
		DrawStyle Style;
		bool UsingVertexColors;
		uint32_t IndexCountPerInstance;
		uint32_t InstanceCount;
		uint32_t StartIndexLocation;
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
		void AddModel(Model& model);

		bool NewBatchInfo();
		void Reset();

		void GetVertexData(std::vector<Vertex>& verts, std::vector<Index>& indices);
		void GetBatchData(std::vector<Batch>& batch);


	private:
		void CreateNewBatch(MeshID mesh, TextureID texture, DrawStyle style);

		bool m_bNewBatch;
		const uint16_t m_maxInstances;
		std::vector<Vertex> m_vertices;
		std::vector<Index> m_indices;
		std::map<std::pair<MeshID, TextureID>, Batch> m_BatchCache;
	};


}

