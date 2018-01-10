#pragma once
#include <Engine\GraphicsWindow.h>

class WindowApp:
	public Graphics::GraphicsWindow
{
public:
	WindowApp(std::string appName);

protected:
	virtual void Update();
	virtual void Render(const std::shared_ptr<Graphics::GraphicsDevice>& graphics);
	virtual void Resume();
	virtual void Suspend();

private:

	Graphics::Text t;
	Graphics::Text mouset;
	Graphics::Model m;
	Graphics::Square s;
};

