#include "Model.h"
#include <glm\gtx\transform.hpp>

std::vector<int> Graphics::IDManager::currentIDs = std::vector<int>();

int Graphics::IDManager::AssignID()
{
	int newID = -1;
	for (int index = 0; index < currentIDs.size(); index++)
	{
		if (currentIDs[index] == -1)
		{
			return index;
		}
		else if (currentIDs[index] > newID)
			newID = currentIDs[index];
	}
	newID = newID + 1;
	currentIDs.push_back(newID);

	return newID;
}

void Graphics::IDManager::RemoveID(int id)
{
	for (auto x : currentIDs)
	{
		if (x == id)
			x = -1;
	}
}

Graphics::Model::Model()
{
	SetMesh(-1);
	SetTexture(-1);
	m_drawStyle = DrawStyle::POINT_3D;
	m_uniqueID = IDManager::AssignID();
	Initialize();
}

Graphics::Model::Model(const MeshID id)
{
	SetMesh(id);
	SetTexture(-1);
	m_drawStyle = DrawStyle::POINT_3D;
	m_uniqueID = IDManager::AssignID();
	Initialize();
}

Graphics::Model::~Model()
{
	IDManager::RemoveID(m_uniqueID);
}

void Graphics::Model::SetTexture(TextureID id)
{
	m_textureID = id;
}

void Graphics::Model::SetMesh(MeshID id)
{
	m_meshID = id;
}

void Graphics::Model::SetDrawStyle(DrawStyle style)
{
	m_drawStyle = style;
}

Graphics::TextureID Graphics::Model::GetTexture()
{
	return m_textureID;
}

Graphics::MeshID Graphics::Model::GetMesh()
{
	return m_meshID;
}

int Graphics::Model::GetUniqueID()
{
	return m_uniqueID;
}

Graphics::DrawStyle Graphics::Model::GetDrawStyle()
{
	return m_drawStyle;
}

void Graphics::Model::Scale(const float x, const float y, const float z)
{
	Scale(glm::vec3(x, y, z));
}

void Graphics::Model::Scale(glm::vec3 scale)
{
	m_scale = scale;
	m_scaleMatrix = glm::scale(m_scale);
}

void Graphics::Model::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Graphics::Model::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Graphics::Model::Rotate(float x, float y, float z)
{
	RotateMatrix(m_outRotation, glm::vec3(x, y, z));
}

void Graphics::Model::Rotate(glm::vec3 rotation)
{
	RotateMatrix(m_outRotation, rotation);
}

void Graphics::Model::RotateAtOrigin(float x, float y, float z)
{
	RotateMatrix(m_inRotation, glm::vec3(x, y, z));
}

void Graphics::Model::SetColor(float r, float g, float b)
{
	m_color = glm::vec4(r, g, b, 1.0f);
}

void Graphics::Model::SetColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void Graphics::Model::SetColor(glm::vec4 color)
{
	m_color = color;
}

void Graphics::Model::SetOpacity(float alpha)
{
	m_color.a = alpha;
}

glm::vec3 Graphics::Model::GetPosition()
{
	return m_position;
}

glm::vec3 Graphics::Model::GetScale()
{
	return m_scale;
}

glm::mat4 Graphics::Model::GetModelMatrix()
{
	CreateLocalToWorld();
	return m_localToWorld;
}

glm::vec4 Graphics::Model::GetColor()
{
	return m_color;
}

void Graphics::Model::Initialize()
{
	m_outRotation = glm::mat4();
	m_inRotation = glm::mat4();
	m_scaleMatrix = glm::mat4();
	m_translation = glm::mat4();
	m_localToWorld = glm::mat4();
	m_position = { 0.0f, 0.0f, 0.0f };
	m_scale = { 1.0f, 1.0f, 1.0f };
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void Graphics::Model::CreateLocalToWorld()
{
	glm::vec3 pos = m_position;
	m_translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	m_localToWorld = m_inRotation * m_translation * m_outRotation * m_scaleMatrix;
}

void Graphics::Model::RotateMatrix(glm::mat4 & matrix, glm::vec3 rot)
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