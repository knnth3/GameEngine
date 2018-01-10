#include "Model.h"
#include <glm\gtx\transform.hpp>

std::vector<int> Graphics::IDManager::currentIDs = std::vector<int>();

void Graphics::IDManager::AssignID(int& id)
{
	int newID = -1;
	for (int index = 0; index < currentIDs.size(); index++)
	{
		if (currentIDs[index] == -1)
		{
			id = index;
			return;
		}
		else if (currentIDs[index] > newID)
			newID = currentIDs[index];
	}
	newID = newID + 1;
	currentIDs.push_back(newID);
	id = newID;
}

void Graphics::IDManager::RemoveID(int id)
{
	for (auto x : currentIDs)
	{
		if (x == id)
			x = -1;
	}
}

Graphics::Model::Model():
	Model(-1)
{
}

Graphics::Model::Model(MeshID mesh)
{
	IDManager::AssignID(m_uniqueID);
	m_mesh            = mesh;
	m_texture         = "";
	m_drawStyle       = TRIANGLE_3D;
	m_rssStyle        = RSS_DEFAULT;
	m_bUseViewMatrix  = true;
	m_scaleMatrix     = glm::mat4();
	m_position        = vec3_ptr(new glm::vec3(0.0f, 0.0f, 0.0f));
	m_scale           = { 1.0f, 1.0f, 1.0f };
	m_object_rotation = { 0.0f, 0.0f, 0.0f };
	m_world_rotation  = { 0.0f, 0.0f, 0.0f };
	m_color           = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_textureBounds   = { 1.0f, 1.0f, 0.0f, 0.0f };
}

Graphics::Model::~Model()
{
	IDManager::RemoveID(m_uniqueID);
}

void Graphics::Model::SetTexture(const std::string& textureName)
{
	m_texture = textureName;
}

void Graphics::Model::SetMesh(MeshID meshName)
{
	m_mesh = meshName;
}

void Graphics::Model::SetDrawStyle(DrawStyle style)
{
	m_drawStyle = style;
}

void Graphics::Model::SetRSS(RSS_STYLES style)
{
	m_rssStyle = style;
}

std::string Graphics::Model::GetTexture()const
{
	return m_texture;
}

Graphics::MeshID Graphics::Model::GetMesh()const
{
	return m_mesh;
}

int Graphics::Model::GetUniqueID()const
{
	return m_uniqueID;
}

Graphics::DrawStyle Graphics::Model::GetDrawStyle()const
{
	return m_drawStyle;
}

Graphics::RSS_STYLES Graphics::Model::GetRSS()const
{
	return m_rssStyle;
}

void Graphics::Model::UseViewMatrix(bool val)
{
	m_bUseViewMatrix = val;
}

bool Graphics::Model::IsUsingViewMatrix()
{
	return m_bUseViewMatrix;
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
	m_position->x = x;
	m_position->y = y;
	m_position->z = z;
}

void Graphics::Model::SetPosition(glm::vec3 pos)
{
	*m_position = pos;
}

void Graphics::Model::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(glm::vec3(x, y, z));
}

void Graphics::Model::SetRelativeRotation(glm::vec3 rotation)
{
	m_object_rotation = rotation;
}

void Graphics::Model::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(glm::vec3(x, y, z));
}

void Graphics::Model::SetWorldRotation(glm::vec3 rotation)
{
	m_world_rotation = rotation;
}

void Graphics::Model::RotateRelative(float x, float y, float z)
{
	RotateRelative(glm::vec3(x, y, z));
}

void Graphics::Model::RotateRelative(glm::vec3 rotation)
{
	m_object_rotation += rotation;

	if (m_object_rotation.x > (float)M_PI)
		m_object_rotation.x -= (float)M_PI;

	if (m_object_rotation.y > (float)M_PI)
		m_object_rotation.y -= (float)M_PI;

	if (m_object_rotation.z > (float)M_PI)
		m_object_rotation.z -= (float)M_PI;
}

void Graphics::Model::RotateWorld(float x, float y, float z)
{
	RotateWorld(glm::vec3(x,y,z));
}

void Graphics::Model::RotateWorld(glm::vec3 rotation)
{
	m_world_rotation += rotation;

	if (m_world_rotation.x > (float)M_PI)
		m_world_rotation.x -= (float)M_PI;

	if (m_world_rotation.y > (float)M_PI)
		m_world_rotation.y -= (float)M_PI;

	if (m_world_rotation.z > (float)M_PI)
		m_world_rotation.z -= (float)M_PI;
}

void Graphics::Model::SetColor(float r, float g, float b)
{
	SetColor(r, g, b, m_color.a);
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

void Graphics::Model::SetTextureBounds(float length, float width, float xoffset, float yoffset)
{
	m_textureBounds = { length, width, xoffset, yoffset };
}

const vec3_ptr Graphics::Model::GetPosition()const 
{
	return m_position;
}

glm::vec3 Graphics::Model::GetScale()const
{
	return m_scale;
}

glm::mat4 Graphics::Model::GetModelMatrix()const
{
	glm::vec3 pos = *m_position;
	glm::mat4 relativeRotation = GetRotationMatrix(m_object_rotation);
	glm::mat4 worldRotation = GetRotationMatrix(m_world_rotation);
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 localToWorld = worldRotation * translation * relativeRotation * m_scaleMatrix;
	return localToWorld;
}

glm::vec4 Graphics::Model::GetColor()const
{
	return m_color;
}

glm::vec4 Graphics::Model::GetTextureBounds()const
{
	return m_textureBounds;
}

glm::mat4 Graphics::Model::GetRotationMatrix(glm::vec3 rotation) const
{
	glm::mat4 matrix = glm::mat4();
	if (rotation.x != 0.0f)
	{
		glm::vec3 rotaxis(1.0f, 0.0f, 0.0f);
		matrix = glm::rotate(matrix, rotation.x, rotaxis);
	}
	if (rotation.y != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 1.0f, 0.0f);
		matrix = glm::rotate(matrix, rotation.y, rotaxis);
	}
	if (rotation.z != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 0.0f, 1.0f);
		matrix = glm::rotate(matrix, rotation.z, rotaxis);
	}

	return matrix;
}