#pragma once
#include "GameState.h"
#include "Engine/Shape_Square.h"

class Game1 :
	public Engine::GraphicsWindow
{
public:
	Game1(std::string appName, float width, float height);

protected:
	virtual void Update();
	virtual void Render(const Engine::EGraphics& graphics);
	virtual void Resume();
	virtual void Suspend();

private:

	enum class GameType
	{
		RUNNING,
		SERVER,
		CLIENT
	};

	void HandleBasicControls(double elapsed);
	void UpdateGamestate();

	int m_bgImageID;
	GameType m_gametype;
	Engine::Model m_skybox;
	Engine::Square m_mainMenu;
	std::unique_ptr<GameState> m_gamestate;
};

