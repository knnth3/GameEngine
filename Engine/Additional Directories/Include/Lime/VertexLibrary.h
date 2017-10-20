#pragma once
#include "Model3D.h"
#include "Model2D.h"

namespace Lime
{
	namespace Model
	{
		class VertexLibrary
		{
		public:
			AppDLL_API bool empty();
			AppDLL_API void clear();
			AppDLL_API size_t size();
			AppDLL_API size_t size2D();
			AppDLL_API std::shared_ptr<Model::Model2D>& at2D(const size_t index);
			AppDLL_API void Add3DModel(std::shared_ptr<Model::Model3D>& model);
			AppDLL_API void Add2DModel(std::shared_ptr<Model::Model2D>& model);
			AppDLL_API const void* VertexData();
			AppDLL_API const void* IndexData();
			AppDLL_API uint32_t VertexDataSize();
			AppDLL_API uint32_t IndexDataSize();

			//Operator overloads
			AppDLL_API std::shared_ptr<Model::Model3D>& operator[] (const size_t index);

		private:
			int VertCountOffset = 0;
			int IndCountOffset = 0;
			std::vector<uint32_t> m_cachedIDs;
			std::vector<Vertex> m_vertices;
			std::vector<uint32_t> m_indices;
			std::vector<std::shared_ptr<Model::Model3D>> m_3Dmodels;
			std::vector<std::shared_ptr<Model::Model2D>> m_2Dmodels;
		};
	}
}