#include "Model.h"
#include "MeshLoader.h"
#include <glm\gtx\transform.hpp>

#define ANGLE_2PI (float)M_PI * 2.0f

std::vector<int> Engine::IDManager::currentIDs = std::vector<int>();

void Engine::IDManager::AssignID(int& id)
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

void Engine::IDManager::RemoveID(int id)
{
	for (auto x : currentIDs)
	{
		if (x == id)
			x = -1;
	}
}

Engine::Model::Model():
	Model(-1)
{
}

Engine::Model::Model(int meshID)
{
	IDManager::AssignID(m_objectID);
	m_meshID          = meshID;
	m_meshID2         = -2;
	m_texture         = "";
	m_drawStyle       = DRAW_STYLE_NORMAL;
	m_bUseViewMatrix  = true;
	m_scaleMatrix     = glm::mat4();
	m_position        = vec3_ptr(new glm::vec3(0.0f, 0.0f, 0.0f));
	m_scale           = { 1.0f, 1.0f, 1.0f };
	m_object_rotation = { 0.0f, 0.0f, 0.0f };
	m_world_rotation  = { 0.0f, 0.0f, 0.0f };
	m_color           = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_textureBounds   = { 1.0f, 1.0f, 0.0f, 0.0f };

	InitializeAnimator();
}

Engine::Model::~Model()
{
	IDManager::RemoveID(m_objectID);
}

void Engine::Model::SetTexture(const std::string& textureName)
{
	m_texture = textureName;
}

void Engine::Model::SetMesh(int ID)
{
	m_meshID = ID;
	InitializeAnimator();
}

void Engine::Model::SetSecondaryMesh(int ID)
{
	m_meshID2 = ID;
}

void Engine::Model::SetDrawStyle(int style)
{
	m_drawStyle = style;
}

std::string Engine::Model::GetTexture()const
{
	return m_texture;
}

int Engine::Model::GetMesh()const
{
	return m_meshID;
}

int Engine::Model::GetSecondaryMesh() const
{
	return m_meshID2;
}

const Engine::AnimTransformPtr Engine::Model::GetTransforms() const
{
	if (m_animator)
		return m_animator->GetTransforms();
	else
		return nullptr;
}

int Engine::Model::GetObjectID()const
{
	return m_objectID;
}

int Engine::Model::GetDrawStyle()const
{
	return m_drawStyle;
}

void Engine::Model::UseViewMatrix(bool val)
{
	m_bUseViewMatrix = val;
}

bool Engine::Model::IsUsingViewMatrix()
{
	return m_bUseViewMatrix;
}

void Engine::Model::Scale(const float x, const float y, const float z)
{
	Scale(glm::vec3(x, y, z));
}

void Engine::Model::Scale(glm::vec3 scale)
{
	m_scale = scale;
	m_scaleMatrix = glm::scale(m_scale);
}

void Engine::Model::SetPosition(const float x, const float y, const float z)
{
	m_position->x = x;
	m_position->y = y;
	m_position->z = z;
}

void Engine::Model::SetPosition(glm::vec3 pos)
{
	*m_position = pos;
}

void Engine::Model::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(glm::vec3(x, y, z));
}

void Engine::Model::SetRelativeRotation(glm::vec3 rotation)
{
	m_object_rotation = rotation;
}

void Engine::Model::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(glm::vec3(x, y, z));
}

void Engine::Model::SetWorldRotation(glm::vec3 rotation)
{
	m_world_rotation = rotation;
}

void Engine::Model::RotateRelative(float x, float y, float z)
{
	RotateRelative(glm::vec3(x, y, z));
}

void Engine::Model::RotateRelative(glm::vec3 rotation)
{
	Rotate(m_object_rotation.x, rotation.x);
	Rotate(m_object_rotation.y, rotation.y);
	Rotate(m_object_rotation.z, rotation.z);
}

void Engine::Model::RotateWorld(float x, float y, float z)
{
	RotateWorld(glm::vec3(x,y,z));
}

void Engine::Model::RotateWorld(glm::vec3 rotation)
{
	Rotate(m_world_rotation.x, rotation.x);
	Rotate(m_world_rotation.y, rotation.y);
	Rotate(m_world_rotation.z, rotation.z);
}

void Engine::Model::SetColor(float r, float g, float b)
{
	SetColor(r, g, b, m_color.a);
}

void Engine::Model::SetColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void Engine::Model::SetColor(glm::vec4 color)
{
	m_color = color;
}

void Engine::Model::SetOpacity(float alpha)
{
	m_color.a = alpha;
}

void Engine::Model::SetTextureBounds(float length, float width, float xoffset, float yoffset)
{
	m_textureBounds = { length, width, xoffset, yoffset };
}

void Engine::Model::Update(double elapsed)
{
	if(m_animator)
		m_animator->Update(elapsed);
}

void Engine::Model::ToggleAnimation()
{
	if(m_animator)
		m_animator->TogglePlay();
}

bool Engine::Model::SetAnimation(int index)
{
	if (m_animator)
		return m_animator->SetAnimation(index);

	return false;
}

void Engine::Model::SetAnimationDelay(int index, bool value)
{
	if (m_animator)
		m_animator->WaitToFinish(index, value);
}

const vec3_ptr Engine::Model::GetPosition()const 
{
	return m_position;
}

glm::vec3 Engine::Model::GetScale()const
{
	return m_scale;
}

glm::mat4 Engine::Model::GetModelMatrix()const
{
	glm::vec3 pos = *m_position;
	glm::mat4 relativeRotation = GetRotationMatrix(m_object_rotation);
	glm::mat4 worldRotation = GetRotationMatrix(m_world_rotation);
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 localToWorld = worldRotation * translation * relativeRotation * m_scaleMatrix;
	return localToWorld;
}

glm::vec4 Engine::Model::GetColor()const
{
	return m_color;
}

glm::vec4 Engine::Model::GetTextureBounds()const
{
	return m_textureBounds;
}

int Engine::Model::GetNumAnimations() const
{
	if (m_animator)
		return m_animator->GetAnimationCount();

	return 0;
}

void Engine::Model::operator=(Model & m)
{
	IDManager::RemoveID(m_objectID);
	m_objectID = m.m_objectID;
	m_meshID = m.m_meshID;
	m_meshID2 = m.m_meshID2;
	m_animator = m.m_animator;
}

void Engine::Model::Rotate(float & original, const float & amount)
{
	original += amount;

	if (original > ANGLE_2PI)
		original -= ANGLE_2PI;

	if (original < -ANGLE_2PI)
		original += ANGLE_2PI;
}

glm::mat4 Engine::Model::GetRotationMatrix(glm::vec3 rotation) const
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

void Engine::Model::InitializeAnimator()
{
	if (m_meshID != -1)
	{
		std::shared_ptr<Mesh> mesh;
		MeshLoader::GrabMeshData(m_meshID, mesh);
		m_animator = std::make_shared<Animator>(mesh);
	}
}
