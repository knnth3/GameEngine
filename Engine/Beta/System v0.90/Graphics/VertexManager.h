#pragma once
#include "Model3D.h"
#include <map>

namespace Graphics
{
	struct VertexManagerInfo
	{
		VertexManagerInfo();
		unsigned int vertexOffset;
		unsigned int indexOffset;
		unsigned int size;
		std::shared_ptr<Model::Model3D> model;
	};

	class VertexManager
	{
	public:
		VertexManager();
		void AddModel(VertexManagerInfo& info);


		bool HasNewData();
		void Reset();
		void GetBufferData(std::vector<Model::Vertex>& verts, std::vector<Model::Index>& indices);


	private:
		bool m_bHasNewData;

		std::map<MeshID, VertexManagerInfo> m_meshCache;
		std::vector<Model::Vertex> m_vertices;
		std::vector<Model::Index> m_indices;
	};


}

