#pragma once
#include "Actor.h"

enum CHAR_LOOK_DIRECTION
{
	ACTOR_LOOK_DOWN,
	ACTOR_LOOK_LEFT,
	ACTOR_LOOK_RIGHT,
	ACTOR_LOOK_UP
};


class Character:
	public Actor
{
public:
	Character();
	~Character();

private:
	void SetMoveAnimation(int index);
	void SetRestAnimation(int index);
	virtual void Animate()override;

	CHAR_LOOK_DIRECTION m_lookDir;
};

