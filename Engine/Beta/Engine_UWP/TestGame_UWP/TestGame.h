#pragma once
#include <Lime_Engine\EngineApp.h>

class TestGame:
	public LIME_ENGINE::EngineApp
{
public:
	TestGame(std::shared_ptr<Graphics::GraphicsDevice>& graphicsDevice,
		std::shared_ptr<LIME_ENGINE::InputManager>& inputManager,
		std::shared_ptr<LIME_ENGINE::StepTimer>& timer);

	//Virtual
	virtual void OnInitialize()override;
	virtual void OnUpdate()override;
	virtual void OnRender()override;
	virtual void OnActivated()override;
	virtual void OnDeactivated()override;
	virtual void OnSuspending()override;
	virtual void OnResuming()override;
	virtual void OnWindowMoved()override;
	virtual void OnWindowSizeChanged()override;
	virtual void OnShutdown()override;

private:

	Graphics::Model m_model;
	Graphics::Model m_model2;
	Graphics::Line m_line;
};

