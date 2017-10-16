#pragma once
#include "Shared.h"
#include <Lime\InputManager.h>

class GameState
{
public:
	GameState() = default;
	//Function called every frame
	//Looks for non-zero on exit
	virtual int Update(float time, std::unique_ptr<Lime::InputManager>& input) = 0;

	//State Changes
	virtual void Initialize(int windowWidth, int windowHeight) = 0;
	virtual void Close() = 0;
	virtual void OnWindowResize(int width, int height);
};

