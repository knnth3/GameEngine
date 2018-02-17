#pragma once
#include <Engine\EngineWindow.h>
#include "Character.h"

class GameState
{
public:
	virtual void Initialize() = 0;
	virtual void Update(double elapsed) = 0;
	virtual void HandleControlls(double elapsed, const std::shared_ptr<Engine::Input>& input, const std::shared_ptr<Engine::Camera>& camera) = 0;
	virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics) = 0;
	virtual void Close() = 0;

};

