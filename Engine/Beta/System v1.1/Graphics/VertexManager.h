#pragma once
#include "Model.h"
#include <map>

namespace Graphics
{
	struct VertexManagerInfo
	{
		VertexManagerInfo();
		unsigned int vertexOffset;
		unsigned int indexOffset;
		unsigned int size;
		std::shared_ptr<Model> model;
	};

	class VertexManager
	{
	public:
		VertexManager();
		void AddModel(VertexManagerInfo& info);


		bool HasNewData();
		void Reset();
		void GetBufferData(std::vector<Vertex>& verts, std::vector<Index>& indices);


	private:
		bool m_bHasNewData;

		std::map<MeshID, VertexManagerInfo> m_meshCache;
		std::vector<Vertex> m_vertices;
		std::vector<Index> m_indices;
	};


}

