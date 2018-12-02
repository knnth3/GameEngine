#pragma once
#include <Engine\EngineWindow.h>

class WindowApp:
	public Engine::GraphicsWindow
{
public:
	WindowApp(std::string appName, float width, float height);

protected:
	virtual void Update();
	virtual void Render(const Engine::EGraphics& graphics);
	virtual void Resume();
	virtual void Suspend();

private:

	Engine::Model m_skybox;
	Engine::Model m_model;

};

