#pragma once
#include "DllSettings.h"

namespace Graphics
{

	struct Vertex
	{
		Vertex();
		glm::vec3 m_position;
		glm::vec2 m_uv;
		glm::vec4 m_normal;
		glm::vec3 m_tangent;
		glm::vec3 m_binormal;
		glm::vec3 m_color;
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

	private:
		template<typename T>
		static void Add(const std::string& name);

		static std::vector<std::pair<size_t, std::string>> m_vertex_info;
		static vertex_info_init init;
	};

	template<typename T>
	inline void VertexInfo::Add(const std::string& name)
	{
		std::pair<size_t, std::string> new_pair;
		new_pair.first = sizeof(T);
		new_pair.second = name;
		m_vertex_info.push_back(new_pair);
	}
}