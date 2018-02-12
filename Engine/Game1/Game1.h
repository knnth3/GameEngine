#pragma once
#include "GameState.h"

class Game1:
	public Engine::GraphicsWindow
{
public:
	Game1(std::string appName, float width, float height);

protected:
	virtual void Update();
	virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics);
	virtual void Resume();
	virtual void Suspend();

private:

	void HandleBasicControls(double elapsed);

	Engine::Model m_skybox;
	std::unique_ptr<GameState> m_gamestate;
};

