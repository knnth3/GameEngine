#pragma once
#include <Engine\EngineWindow.h>
#include "Character.h"

class GameState
{
public:
	GameState();
	~GameState();

	void Initialize();
	void Update(double elapsed);
	void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics);
	void Close();

private:

	std::unique_ptr<Character> m_user;
	std::unique_ptr<Character> m_user2;
	Engine::Model m_plane;
};

