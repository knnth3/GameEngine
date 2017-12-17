#pragma once
#include "Actor.h"

class Item:
	public Actor
{
public:
	Item();
	~Item();

private:
	virtual void Animate()override;
};

