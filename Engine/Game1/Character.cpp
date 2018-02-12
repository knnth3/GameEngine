#include "Character.h"
#include "Engine\MeshLoader.h"

using namespace Engine;

#define MOVE_ERROR 10

Character::Character(const CharacterInfo& info)
{
	int bodyID = -1;
	int headID = -2;
	m_bRunning = false;
	m_bMoving = false;
	m_model.Scale(50, 50, 50);
	SetInfo(info);
}

Character::Character(const CharacterInfo& info, const std::string & sefPath, const std::string & sefPathHead)
{
	int bodyID = -1;
	int headID = -2;
	m_bRunning = false;
	m_bMoving = false;
	bodyID = MeshLoader::LoadModel(sefPath);
	if (!sefPathHead.empty())
	{
		headID = MeshLoader::LoadModel(sefPathHead);
	}

	m_model.SetMesh(bodyID);
	m_model.SetSecondaryMesh(headID);
	m_model.SetAnimationDelay(2, true);
	m_model.Scale(50, 50, 50);
	SetInfo(info);
}

Character::~Character()
{
}

void Character::SetInfo(const CharacterInfo & info)
{
	m_info = info;
}

void Character::SetDestination(const glm::vec3 & position)
{
	if (m_destination != position)
	{
		m_destination = position;
		m_bMoving = true;

		//Calculate rotation
		glm::vec3 currentPos = *m_model.GetPosition();
		glm::vec3 path = m_destination - currentPos;

		if (path.x != 0.0f)
		{
			float rotation = atanf(-path.z / path.x);
			double offset = 0.0f;
			if (path.x < 0.0f)
			{
				offset = M_PI;
			}

			m_model.SetRelativeRotation(0.0f, (rotation + float(M_PI_2 + offset)), 0.0f);
		}
	}
}

void Character::ToggleRun()
{
	m_bRunning = !m_bRunning;
}

void Character::Update(double elapsed)
{
	SetAnimation();
	double seconds = elapsed / 1000.0;
	m_model.Update(seconds);
	Move(elapsed);
}

const Engine::Model & Character::GetModel() const
{
	return m_model;
}

const std::string & Character::GetName() const
{
	return m_name;
}

void Character::SetAnimation()
{
	if (!m_bMoving)
	{
		m_model.SetAnimation(0);
	}
	else if(!m_bRunning)
	{
		m_model.SetAnimation(1);
	}
}

void Character::Move(double elapsed)
{
	if (m_bMoving)
	{
		glm::vec3 position = *m_model.GetPosition();
		glm::vec3 path = m_destination - position;
		float distance = glm::length(path);
		if (distance > (m_info.Speed + MOVE_ERROR))
		{
			glm::vec3 newPosition = position + (m_info.Speed * glm::normalize(path) * (float)elapsed);
			m_model.SetPosition(newPosition);
		}
		else
		{
			m_model.SetPosition(m_destination);
			m_bMoving = false;
		}
	}
}
