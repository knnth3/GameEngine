#include "pch.h"
#include "Character.h"
#include <Lime_Engine\EngineResources.h>

using namespace Graphics;
using namespace LIME_ENGINE;

Character::Character()
{
	m_lookDir = ACTOR_LOOK_DOWN;
	SetHeightOffset(-BLOCK_HEIGHT * 0.25f);
}

Character::~Character()
{
}

void Character::SetMoveAnimation(int index)
{
	glm::vec2 position = GetPosition();
	glm::vec2 destination = GetDestination();
	glm::vec2 direction = glm::normalize(destination - position);
	if (direction.y < 0)
	{
		m_lookDir = ACTOR_LOOK_UP;
		SetImageAtlasPosition(index, 1);
	}
	else if (direction.y > 0)
	{
		m_lookDir = ACTOR_LOOK_DOWN;
		SetImageAtlasPosition(index, 0);
	}
	else if (direction.x > 0)
	{
		m_lookDir = ACTOR_LOOK_RIGHT;
		SetImageAtlasPosition(index, 3);
	}
	else if (direction.x < 0)
	{
		m_lookDir = ACTOR_LOOK_LEFT;
		SetImageAtlasPosition(index, 2);
	}
}

void Character::SetRestAnimation(int index)
{
	switch (m_lookDir)
	{
	case ACTOR_LOOK_DOWN:
		SetImageAtlasPosition(index, 0);
		break;
	case ACTOR_LOOK_LEFT:
		SetImageAtlasPosition(index, 2);
		break;
	case ACTOR_LOOK_RIGHT:
		SetImageAtlasPosition(index, 3);
		break;
	case ACTOR_LOOK_UP:
		SetImageAtlasPosition(index, 1);
		break;
	default:
		break;
	}
}

void Character::Animate()
{
	double deltatime = EngineResources::GetTimer()->GetElapsedSeconds();
	static int frame = 0;
	static double totalTime = deltatime;
	static bool bEnteredRest = false;
	if (IsMoving())
	{
		if (totalTime > ANIM_DURATION)
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
		if (!bEnteredRest && totalTime > ANIM_DURATION)
		{
			totalTime = 0.0;
			SetRestAnimation(0);
			bEnteredRest = true;
		}
	}
	totalTime += deltatime;
}
