#pragma once
#include "DllSettings.h"
#include "TextureManager.h"


namespace Graphics
{
	namespace Model
	{
		typedef uint32_t Index;

		class IDManager
		{
		public:
			static int AssignID();
			static void RemoveID(int id);

		private:
			static std::vector<int> currentIDs;
		};

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
		public:
			DLL_API const unsigned int GetVertexCount();
			DLL_API const unsigned int GetIndexCount();
			std::vector<uint32_t> m_indices;
			std::vector<T> m_vertices;
		};

		class MeshData
		{
		public:
			DLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
			DLL_API uint32_t GetIndexCount();
			DLL_API float GetHeight();
			uint32_t objectID = 0;
			uint32_t vertOffset = 0;
			uint32_t indiciOffset = 0;
			std::vector<Polygon<Vertex>> m_polygons;
			float m_height = 0.0f;
		};

		class MeshData2D
		{
		public:
			DLL_API MeshData2D();
			DLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices);
			DLL_API uint32_t GetIndexCount();

			uint32_t vertOffset = 0;
			uint32_t indiciOffset = 0;
			std::vector<Polygon<Vertex>> m_polygons;
		};
	}
}