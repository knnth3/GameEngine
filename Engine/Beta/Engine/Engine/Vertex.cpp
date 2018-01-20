#include "Vertex.h"
#define nameof(name) #name

std::vector<std::tuple<size_t, std::string, Engine::DType>> Engine::VertexInfo::m_vertex_info;
Engine::vertex_info_init Engine::VertexInfo::init;

Engine::Vertex::Vertex()
{
	m_color = glm::vec3(1.0f);
	for (auto x = 0; x < MAX_JOINTS_PER_VERTEX; x++)
	{
		m_jointIDs[x] = 0;
		m_jointWeights[x] = 0.0f;
	}
}

void Engine::VertexInfo::Initialize()
{
	static bool posted = false;
	//floats or ints only
	if (!posted)
	{
		Vertex temp;
		VertexInfo::Add("POSITION", sizeof(temp.m_position), DType::FLOAT);
		VertexInfo::Add("TEXCOORD", sizeof(temp.m_uv), DType::FLOAT);
		VertexInfo::Add("NORMAL", sizeof(temp.m_normal), DType::FLOAT);
		VertexInfo::Add("TANGENT", sizeof(temp.m_tangent), DType::FLOAT);
		VertexInfo::Add("BINORMAL", sizeof(temp.m_binormal), DType::FLOAT);
		VertexInfo::Add("COLOR", sizeof(temp.m_color), DType::FLOAT);
		VertexInfo::Add("JOINT_ID", sizeof(temp.m_jointIDs), DType::UINT);
		VertexInfo::Add("JOINT_WEIGHT", sizeof(temp.m_jointWeights), DType::FLOAT);
		posted = true;
	}
}

size_t Engine::VertexInfo::NumElements()
{
	return m_vertex_info.size();
}

const std::string* Engine::VertexInfo::NameOf(size_t index)
{
	return &std::get<1>(m_vertex_info[index]);
}

size_t Engine::VertexInfo::SizeOf(size_t index)
{
	return std::get<0>(m_vertex_info[index]);
}

Engine::DType Engine::VertexInfo::TypeOf(size_t index)
{
	return std::get<2>(m_vertex_info[index]);
}

void Engine::VertexInfo::Add(const std::string & name, int size, DType type)
{
	m_vertex_info.emplace_back(size, name, type);
}

Engine::vertex_info_init::vertex_info_init()
{
	VertexInfo::Initialize();
}