#pragma once
#include <Lime_Engine\EngineApp.h>

class TestGame:
	public LIME_ENGINE::EngineApp
{
public:
	TestGame(std::shared_ptr<Graphics::GraphicsDevice>& graphicsDevice,
		std::shared_ptr<LIME_ENGINE::InputManager>& inputManager,
		std::shared_ptr<LIME_ENGINE::StepTimer>& timer);

	//Required
	virtual void OnUpdate()override;
	virtual void OnRender()override;
	virtual void OnResume()override;
	virtual void OnSuspend()override;
	virtual void OnWindowSizeChanged()override;

private:

	Graphics::Model m_model;
	Graphics::Model m_model2;
	Graphics::Line m_line;
};

