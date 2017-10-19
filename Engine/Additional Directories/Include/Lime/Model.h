#pragma once
#include "Shared.h"
#include "TextureManager.h"


namespace Lime
{
	//Forward decls in Lime
	class TextInfo;
	class DX11Graphics;
	namespace Model
	{
		//Enum to identify different model uses
		enum MeshType
		{
			NONE = -1,
			TEXT = 0,
			TRIANGLE = 1,
			POINT = 2,
			LINE = 3
		};

		struct Vertex
		{
			glm::vec3 m_position;
			glm::vec2 m_uv;
			glm::vec3 m_normal;
		};

		//-Class that handles each Polygon face on a mesh
		//-Max Vertices == sizeof(uint32_t)
		template<class T>
		class Polygon
		{
			friend class MeshData;
			friend class MeshData2D;
			friend class MeshLoader;
			friend class VertexLibrary;
		public:
			AppDLL_API c_uint GetVertexCount();
			AppDLL_API c_uint GetIndexCount();

		protected:
			std::vector<uint32_t> m_indices;
			std::vector<T> m_vertices;
		};

		class MeshData
		{
			friend class MeshLoader;
		public:
			AppDLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
			AppDLL_API uint32_t GetIndexCount();
			AppDLL_API float GetHeight();
			uint32_t objectID = 0;
			uint32_t vertOffset = 0;
			uint32_t indiciOffset = 0;
		protected:
			std::vector<Polygon<Vertex>> m_polygons;
			float m_height = 0.0f;
		};

		class MeshData2D
		{
			friend class Model2D;
			friend class VertexLibrary;
		public:
			AppDLL_API MeshData2D();
			AppDLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices);
			AppDLL_API uint32_t GetIndexCount();

			uint32_t vertOffset = 0;
			uint32_t indiciOffset = 0;
		protected:
			std::vector<Polygon<Vertex>> m_polygons;
		};
	}
}