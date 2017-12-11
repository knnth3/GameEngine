#include "pch.h"
#include "Actor.h"
#include <Lime_Engine\EngineResources.h>


#define MOVE_SPEED 245.0f
#define ANIM_SPEED 0.15

using namespace Graphics;
using namespace LIME_ENGINE;

Actor::Actor()
{
	SetDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	m_bNewCommands = false;
	m_bisMoving = false;
	m_lookDir = ACTOR_LOOK_DOWN;
}

Actor::Actor(std::string name)
{
	SetName(name);
	SetDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	m_bisMoving = false;
	m_bNewCommands = false;
	m_lookDir = ACTOR_LOOK_DOWN;
}

void Actor::Update()
{
	Animate();
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
		if (totalDistance > (1.0f * MOVE_SPEED * deltatime) )
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

std::string Actor::GetName()const
{
	return m_name;
}

glm::vec2 Actor::GetPosition() const
{
	return m_gridPos;
}

glm::vec2 Actor::GetModelPosition() const
{
	auto pos = m_model.GetPosition();
	pos.x += (BLOCK_WIDTH * 0.5f);
	pos.y += (BLOCK_WIDTH * 0.5f);
	return pos;
}

bool Actor::IsMoving() const
{
	return m_bisMoving;
}

const Graphics::Square & Actor::Get()const
{
	return m_model;
}

void Actor::GetDirection()
{

}

void Actor::SetMoveAnimation(int index)
{
	glm::vec2 position = m_model.GetPosition();
	glm::vec2 destination = GridToWorld(m_destination);
	glm::vec2 direction = glm::normalize(destination - position);
	if (direction.y < 0)
	{
		m_lookDir = ACTOR_LOOK_UP;
		m_model.SetImageAtlasPosition(index, 1);
	}
	else if(direction.y > 0)
	{
		m_lookDir = ACTOR_LOOK_DOWN;
		m_model.SetImageAtlasPosition(index, 0);
	}
	else if (direction.x > 0)
	{
		m_lookDir = ACTOR_LOOK_RIGHT;
		m_model.SetImageAtlasPosition(index, 3);
	}
	else if (direction.x < 0)
	{
		m_lookDir = ACTOR_LOOK_LEFT;
		m_model.SetImageAtlasPosition(index, 2);
	}
}

void Actor::SetRestAnimation(int index)
{
	switch (m_lookDir)
	{
	case ACTOR_LOOK_DOWN:
		m_model.SetImageAtlasPosition(index, 0);
		break;
	case ACTOR_LOOK_LEFT:
		m_model.SetImageAtlasPosition(index, 2);
		break;
	case ACTOR_LOOK_RIGHT:
		m_model.SetImageAtlasPosition(index, 3);
		break;
	case ACTOR_LOOK_UP:
		m_model.SetImageAtlasPosition(index, 1);
		break;
	default:
		break;
	}
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

	result.x = (pos.x * BLOCK_WIDTH) - (width * 0.5f) + (BLOCK_WIDTH * 0.5f);
	result.y = (pos.y * BLOCK_HEIGHT) - (height * 0.75f) + (BLOCK_HEIGHT * 0.5F);
	return result;
}

void Actor::Animate()
{
	double deltatime = EngineResources::GetTimer()->GetElapsedSeconds();
	static int frame = 0;
	static double totalTime = deltatime;
	static bool bEnteredRest = false;
	if (m_bisMoving)
	{
		if (totalTime > ANIM_SPEED)
		{
			frame++;
			frame = frame % 8;
			totalTime = 0.0;
			SetMoveAnimation(frame);
		}
		bEnteredRest = false;
	}
	else
	{
		if (!bEnteredRest && totalTime > ANIM_SPEED)
		{
			totalTime = 0.0;
			SetRestAnimation(0);
			bEnteredRest = true;
		}
	}
	totalTime += deltatime;
}