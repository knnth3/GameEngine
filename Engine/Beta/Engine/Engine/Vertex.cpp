#include "Vertex.h"
#define nameof(name) #name

std::vector<std::pair<size_t, std::string>> Graphics::VertexInfo::m_vertex_info;
Graphics::vertex_info_init Graphics::VertexInfo::init;

Graphics::Vertex::Vertex()
{
	m_color = glm::vec3(1.0f);
}

void Graphics::VertexInfo::Initialize()
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

size_t Graphics::VertexInfo::NumElements()
{
	return m_vertex_info.size();
}

const std::string* Graphics::VertexInfo::NameOf(size_t index)
{
	return &m_vertex_info[index].second;
}

size_t Graphics::VertexInfo::SizeOf(size_t index)
{
	return m_vertex_info[index].first;
}

Graphics::vertex_info_init::vertex_info_init()
{
	VertexInfo::Initialize();
}
