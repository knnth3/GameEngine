#include <glm\gtx\transform.hpp>
#include "Model3D.h"



Graphics::Model::Model3D::Model3D()
{
	m_meshID = -1;
	Initialize();
}

Graphics::Model::Model3D::Model3D(const MeshID id)
{
	m_meshID = id;
	Initialize();
}

Graphics::Model::Model3D::~Model3D()
{
	IDManager::RemoveID(m_uniqueID);
}

void Graphics::Model::Model3D::Scale(const float x, const float y, const float z)
{
	Scale(glm::vec3(x, y, z));
}

void Graphics::Model::Model3D::Scale(glm::vec3 scale)
{
	m_scale = scale;
	m_scaleMatrix = glm::scale(m_scale);
}

void Graphics::Model::Model3D::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Graphics::Model::Model3D::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Graphics::Model::Model3D::Rotate(float x, float y, float z)
{
	RotateMatrix(m_outRotation, glm::vec3(x, y, z));
}

void Graphics::Model::Model3D::Rotate(glm::vec3 rotation)
{
	RotateMatrix(m_outRotation, rotation);
}

void Graphics::Model::Model3D::RotateAtOrigin(float x, float y, float z)
{
	RotateMatrix(m_inRotation, glm::vec3(x, y, z));
}

void Graphics::Model::Model3D::SetColor(float r, float g, float b)
{
	m_color = glm::vec4(r, g, b, 1.0f);
}

void Graphics::Model::Model3D::SetColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void Graphics::Model::Model3D::SetColor(glm::vec4 color)
{
	m_color = color;
}

void Graphics::Model::Model3D::SetOpacity(float alpha)
{
	m_color.a = alpha;
}

void Graphics::Model::Model3D::SetTexture(TextureID tex)
{
	m_textureID = tex;
}

void Graphics::Model::Model3D::SetMesh(MeshID id)
{
	m_meshID = id;
}

glm::vec3 Graphics::Model::Model3D::GetPosition() const
{
	return m_position;
}

glm::vec3 Graphics::Model::Model3D::GetScale() const
{
	return m_scale;
}

glm::mat4 Graphics::Model::Model3D::GetModelMatrix()
{
	CreateLocalToWorld();
	return m_localToWorld;
}

glm::vec4 Graphics::Model::Model3D::GetColor()
{
	return m_color;
}

Graphics::TextureID Graphics::Model::Model3D::GetTexture()
{
	return m_textureID;
}

Graphics::MeshID Graphics::Model::Model3D::GetMesh()
{
	return m_meshID;
}

int Graphics::Model::Model3D::GetUniqueID()
{
	return m_uniqueID;
}

void Graphics::Model::Model3D::Initialize()
{
	m_textureID = -1;
	m_outRotation = glm::mat4();
	m_inRotation = glm::mat4();
	m_scaleMatrix = glm::mat4();
	m_translation = glm::mat4();
	m_localToWorld = glm::mat4();
	m_position = { 0.0f, 0.0f, 0.0f };
	m_scale = { 1.0f, 1.0f, 1.0f };
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_uniqueID = IDManager::AssignID();
}

void Graphics::Model::Model3D::CreateLocalToWorld()
{
	glm::vec3 pos = m_position;
	m_translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	m_localToWorld = m_inRotation * m_translation * m_outRotation * m_scaleMatrix;
}

void Graphics::Model::Model3D::RotateMatrix(glm::mat4 & matrix, glm::vec3 rot)
{
	matrix = glm::mat4();
	if (rot.x != 0.0f)
	{
		glm::vec3 rotaxis(1.0f, 0.0f, 0.0f);
		matrix = glm::rotate(matrix, rot.x, rotaxis);
	}
	if (rot.y != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 1.0f, 0.0f);
		matrix = glm::rotate(matrix, rot.y, rotaxis);
	}
	if (rot.z != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 0.0f, 1.0f);
		matrix = glm::rotate(matrix, rot.z, rotaxis);
	}
}