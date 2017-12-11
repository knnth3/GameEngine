#include "pch.h"
#include "Character.h"
#include "Shared.h"

using namespace Lime;

Character::Character(Character_Info info)
{
	m_info = info;
}

void Character::SetModel(MeshID id, shared_ptr<RenderBatch>& rend, shared_ptr<Camera>& camera)
{
	m_model = std::make_shared<Model3D>(id);
	rend->Add3DModel(m_model);
	camera->AttachToModel(m_model);
}

void Character::Update(float time)
{
	this->MovePlayer(time);
	m_model->SetPosition(m_info.position);
}

void Character::Draw()
{
	this->DrawModel();
}

void Character::SetLocation(glm::vec3 position)
{
	m_info.position = position;
	m_info.destination = position;
	m_info.isMoving = false;
}

void Character::SetDestination(glm::vec3 position)
{
	m_info.destination = position;
	m_info.isMoving = true;
}

void Character::MovePlayer(float time)
{
	if (m_info.isMoving)
	{
		glm::vec3 direction = m_info.destination - m_info.position;
		float distance = glm::length(direction);
		float moveDist = m_info.move_speed * time;
		if (distance >= moveDist)
		{
			m_info.position += (glm::normalize(direction) * moveDist);
		}
		else
		{
			m_info.isMoving = false;
			m_info.position = m_info.destination;
		}
	}
}

Character_Info::Character_Info()
{
	isMoving = false;
	move_speed = 5.0f * METER;
	current_health = 100;
	max_health = 100;
	character_name = "None";
	player_name = "None";
	position = {0.0f};
	destination = {0.0f};
}
