#pragma once
#include "DllSettings.h"
#include <tuple>

#define MAX_JOINTS_PER_VERTEX 4

namespace Engine
{
	enum class DType
	{
		FLOAT,
		UINT
	};

	//Holds the information of a single vertex.
	//If changed, make sure to update VertexInfo as well 
	//with the new/deleted members in cpp.
	struct Vertex
	{
		Vertex();
		glm::vec3 m_position;
		glm::vec2 m_uv;
		glm::vec4 m_normal;
		glm::vec3 m_tangent;
		glm::vec3 m_binormal;
		glm::vec3 m_color;
		uint32_t m_jointIDs[MAX_JOINTS_PER_VERTEX];
		float m_jointWeights[MAX_JOINTS_PER_VERTEX];
	};

	struct vertex_info_init
	{
		vertex_info_init();
	};

	class VertexInfo
	{
	public:
		static void Initialize();
		static size_t NumElements();
		static const std::string* NameOf(size_t index);
		static size_t SizeOf(size_t index);
		static DType TypeOf(size_t index);

	private:
		static void Add(const std::string& name, int size, DType type);

		static std::vector<std::tuple<size_t, std::string, DType>> m_vertex_info;
		static vertex_info_init init;
	};
}