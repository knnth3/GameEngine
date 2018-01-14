#include "Vertex.h"
#define nameof(name) #name

std::vector<std::pair<size_t, std::string>> Engine::VertexInfo::m_vertex_info;
Engine::vertex_info_init Engine::VertexInfo::init;

Engine::Vertex::Vertex()
{
	m_color = glm::vec3(1.0f);
}

void Engine::VertexInfo::Initialize()
{
	static bool posted = false;

	if (!posted)
	{
		VertexInfo::Add<glm::vec3>("POSITION");
		VertexInfo::Add<glm::vec2>("TEXCOORD");
		VertexInfo::Add<glm::vec3>("NORMAL");
		VertexInfo::Add<glm::vec4>("TANGENT");
		VertexInfo::Add<glm::vec3>("BINORMAL");
		VertexInfo::Add<glm::vec3>("COLOR");
		posted = true;
	}
}

size_t Engine::VertexInfo::NumElements()
{
	return m_vertex_info.size();
}

const std::string* Engine::VertexInfo::NameOf(size_t index)
{
	return &m_vertex_info[index].second;
}

size_t Engine::VertexInfo::SizeOf(size_t index)
{
	return m_vertex_info[index].first;
}

Engine::vertex_info_init::vertex_info_init()
{
	VertexInfo::Initialize();
}
