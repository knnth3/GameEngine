#pragma once
#include <Engine\EngineWindow.h>

class WindowApp:
	public Engine::GraphicsWindow
{
public:
	WindowApp(std::string appName, float width, float height);

protected:
	virtual void Update();
	virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics);
	virtual void Resume();
	virtual void Suspend();

private:

	Engine::Text t;
	Engine::Model m_floor;
	Engine::Model m_cube1;
	Engine::Model m_cube2;

};

