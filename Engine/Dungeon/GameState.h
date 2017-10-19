#pragma once
#include "Shared.h"
#include <Lime\InputManager.h>

namespace GameStates
{
	enum States
	{
		CLOSE = -1,
		RUNNING = 0,
		MAIN_MENU= 1,
		MAP_EDITOR = 2
	};
}

class GameState
{
public:
	GameState() = default;
	//Function called every frame
	//Looks for non-zero on exit
	virtual GameStates::States Update(float time, std::shared_ptr<Lime::InputManager>& input) = 0;

	//State Changes
	virtual void Initialize(int windowWidth, int windowHeight) = 0;
	virtual void Close() = 0;
	virtual void OnWindowResize(int width, int height);
};

