#pragma once
#include <Engine\EngineWindow.h>
#include "Character.h"

class GameState
{
public:
	virtual void Initialize() = 0;
	virtual void Update(double elapsed) = 0;
	virtual void HandleControlls(double elapsed, const Engine::EInput& input, const Engine::ECamera& camera) = 0;
	virtual void Render(const Engine::EGraphics& graphics) = 0;
	virtual void Close() = 0;

};

