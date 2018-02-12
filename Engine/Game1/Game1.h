#pragma once
#include <Engine\EngineWindow.h>

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

	Engine::Text t;
	Engine::Model m_model;
	Engine::Model m_cube1;
	Engine::Model m_cube2;
};

