#include <glm\gtx\transform.hpp>
#include "Primitives.h"

Lime::ModelData::ModelData() :
	m_Verticies(),
	m_Indicies()
{
}
size_t Lime::ModelData::VertexBufferSize()
{
	return m_Verticies.size() * sizeof(Vertex);
}

size_t Lime::ModelData::IndexBufferSize()
{
	return m_Indicies.size();
}

Lime::Model2::Model2() :
	m_offset(),
	m_Position(),
	m_scaleMatrix(),
	m_translation(),
	m_outRotation(),
	m_inRotation(),
	m_color(1.0f),
	m_resource(0)
{
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_offset = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Data = std::make_shared<ModelData2>();
}

void Lime::Model2::Scale(const float x, const float y, const float z)
{
	m_scale = glm::vec3(x, y, z);
	m_offset.x *= m_scale.x;
	m_offset.y *= m_scale.y;
	m_offset.z *= m_scale.z;
	m_scaleMatrix = glm::scale(m_scale);
}

void Lime::Model2::Translate(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void Lime::Model2::Translate(glm::vec3 pos)
{
	m_Position = pos;
}

void Lime::Model2::Rotate(float x, float y, float z)
{
	m_outRotation = glm::mat4();
	if (x != 0.0f)
	{
		glm::vec3 rotaxis(1.0f, 0.0f, 0.0f);
		m_outRotation = glm::rotate(m_outRotation, x, rotaxis);
	}
	if (y != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 1.0f, 0.0f);
		m_outRotation = glm::rotate(m_outRotation, y, rotaxis);
	}
	if (z != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 0.0f, 1.0f);
		m_outRotation = glm::rotate(m_outRotation, z, rotaxis);
	}
}

void Lime::Model2::RotateAtOrigin(float x, float y, float z)
{
	m_inRotation = glm::mat4();
	if (x != 0.0f)
	{
		glm::vec3 rotaxis(1.0f, 0.0f, 0.0f);
		m_inRotation = glm::rotate(m_inRotation, x, rotaxis);
	}
	if (y != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 1.0f, 0.0f);
		m_inRotation = glm::rotate(m_inRotation, y, rotaxis);
	}
	if (z != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 0.0f, 1.0f);
		m_inRotation = glm::rotate(m_inRotation, z, rotaxis);
	}
}

void Lime::Model2::Color(float r, float g, float b)
{
	m_color = glm::vec4(r, g, b, 1.0f);
}

void Lime::Model2::Color(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void Lime::Model2::SetOpacity(float alpha)
{
	m_color.a = alpha;
}

void Lime::Model2::SetOffset(float offset)
{
	m_offset = glm::vec3(offset * m_scale.x,0.0f, 0.0f);
}

void Lime::Model2::SetTexture(Texture tex)
{
	m_resource = tex;
}

glm::vec3 Lime::Model2::GetPosition() const
{
	return m_Position - m_offset;
}

glm::mat4 Lime::Model2::GetLocalToWorld()
{
	CreateLocalToWorld();
	return m_localToWorld;
}

glm::vec4 Lime::Model2::GetColor()
{
	return m_color;
}

Texture Lime::Model2::GetTexture()
{
	return m_resource;
}

void Lime::Model2::CreateLocalToWorld()
{
	glm::vec3 pos = m_Position - m_offset;
	m_translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	m_localToWorld = m_inRotation * m_translation * m_outRotation * m_scaleMatrix;
}

Lime::Vertex2::Vertex2(Vertex v)
{
	position = v.position;
}

Lime::Vertex2::Vertex2(float x, float y, float z, float u, float v)
{
	position = { x,y,z };
	uv = { u,v };
}

Lime::ModelData2::ModelData2() :
	m_Verticies(),
	m_Indicies()
{
	m_ObjectID = ModelData2::GetNewID();
}
size_t Lime::ModelData2::VertexBufferSize()
{
	return m_Verticies.size() * sizeof(Vertex);
}

size_t Lime::ModelData2::IndexBufferSize()
{
	return m_Indicies.size();
}

const unsigned int Lime::ModelData2::GetNewID()
{
	static unsigned int GlobalIDs = 0;
	return GlobalIDs++;
}