#include "pch.h"
#include "Actor.h"
#include <Lime_Engine\EngineResources.h>


#define MOVE_SPEED 245.0f

using namespace Graphics;
using namespace LIME_ENGINE;

Actor::Actor()
{
	SetDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	SetHeightOffset(0.0f);
	m_bNewCommands = false;
	m_bisMoving = false;
	m_bPassable = true;
}

Actor::Actor(std::string name)
{
	SetName(name);
	SetDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	SetHeightOffset(0.0f);
	m_bNewCommands = false;
	m_bisMoving = false;
	m_bPassable = true;
}

void Actor::Update()
{
	Move();
	Animate();
}

void Actor::SetName(std::string name)
{
	m_name = name;
}

void Actor::SetDestination(glm::vec2 pos)
{
	if (pos != m_gridPos)
	{
		m_bisMoving = true;
		//m_destination = pos;
	}
}

void Actor::SetPosition(glm::vec2 pos)
{
	m_gridPos = pos;	
	m_model.SetPosition(GridToWorld(pos));
}

void Actor::SetPosition(float width, float height)
{
	SetPosition(glm::vec2(width, height));
}

void Actor::SetBrush(int ID, BRUSH_TEXTURE_TYPE type)
{
	m_model.SetBrush(ID, type);
}

void Actor::SetImageAtlasDivisons(int x, int y)
{
	m_model.SetImageAtlasDivisons(x, y);
	SetPosition(GetPosition());
}

void Actor::SetImageAtlasPosition(int x, int y)
{
	m_model.SetImageAtlasPosition(x, y);
}

void Actor::SetDimensions(float x, float y)
{
	m_model.SetDimensions(x, y);
	SetPosition(GetPosition());
}

void Actor::SetMoveInstructions(NodePositions positions)
{
	m_bNewCommands = true;
	m_moves = positions;
}

void Actor::SetPassableState(bool val)
{
	m_bPassable = val;
}

std::string Actor::GetName()const
{
	return m_name;
}

glm::vec2 Actor::GetPosition() const
{
	return m_gridPos;
}

glm::vec2 Actor::GetDestination() const
{
	return m_destination;
}

glm::vec2 Actor::GetModelPosition() const
{
	auto pos = m_model.GetPosition();
	pos.x += (BLOCK_WIDTH * 0.5f);
	pos.y += (BLOCK_WIDTH * 0.5f);
	return pos;
}

const Graphics::Square & Actor::Get()const
{
	return m_model;
}

bool Actor::IsPassable() const
{
	return m_bPassable;
}

Vector2<int> Actor::GetImageAtlasDivisons()const
{
	Vector2<int> ret;
	ret.x = m_model.GetImageAtlasDivisionWidth();
	ret.y = m_model.GetImageAtlasDivisionHeight();
	return ret;
}

Vector2<int> Actor::GetImageAtlasPosition()const
{
	Vector2<int> ret;
	ret.x = m_model.GetImageAtlasPositionX();
	ret.y = m_model.GetImageAtlasPositionY();
	return ret;
}

void Actor::SetHeightOffset(float offset)
{
	m_heightOffset = offset;
	auto newPos = GridToWorld(GetPosition());
	m_model.SetPosition(newPos);
}

void Actor::Move()
{
	float deltatime = (float)EngineResources::GetTimer()->GetElapsedSeconds();
	if (!m_moves.empty())
	{
		if (!m_bisMoving)
		{
			m_destination = glm::vec2(m_moves[0].x, m_moves[0].y);
			m_bisMoving = true;
		}
		glm::vec2 position = m_model.GetPosition();
		glm::vec2 destination = GridToWorld(m_destination);
		float totalDistance = glm::distance(position, destination);
		if (totalDistance > (1.0f * MOVE_SPEED * deltatime))
		{
			glm::vec2 direction = glm::normalize(destination - position);
			direction *= MOVE_SPEED * deltatime;
			glm::vec2 newPos = position + direction;
			m_model.SetPosition(newPos);
		}
		else
		{
			m_model.SetPosition(destination);
			m_gridPos = m_destination;

			if (m_bNewCommands)
				m_bNewCommands = false;
			else
				m_moves.pop_front();

			m_bisMoving = false;
		}
	}
}

float Actor::GetHeightOffset()const
{
	return m_heightOffset;
}

bool Actor::IsMoving()const
{
	return m_bisMoving;
}

glm::vec2 Actor::GridToWorld(Vector2<float> pos)
{
	return GridToWorld(glm::vec2(pos.x, pos.y));
}

glm::vec2 Actor::GridToWorld(glm::vec2 pos)
{
	glm::vec2 result;
	//Align image to grid
	float width = m_model.GetWidth();
	float height = m_model.GetHeight();
	float offset = GetHeightOffset();

	result.x = (pos.x * BLOCK_WIDTH) - (width * 0.5f) + (BLOCK_WIDTH * 0.5f);
	result.y = (pos.y * BLOCK_HEIGHT) - (height * 0.5f) + (BLOCK_HEIGHT * 0.5F) + offset;
	return result;
}