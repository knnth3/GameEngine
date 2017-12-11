#include "EngineApp.h"
#include "EngineResources.h"

using namespace Graphics;


LIME_ENGINE::EngineApp::EngineApp()
{
	m_clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
	m_defaultDimensions = glm::vec2(800.0f, 600.0f);
	m_bActivated = false;
}

void LIME_ENGINE::EngineApp::RegisterGraphicsDevice(std::shared_ptr<Graphics::GraphicsDevice> graphicsDevice)
{
	EngineResources::RegisterGraphicsDevice(graphicsDevice);
}

void LIME_ENGINE::EngineApp::RegisterInputManager(std::shared_ptr<InputManager> inputManager)
{
	EngineResources::RegisterInputManager(inputManager);
}

void LIME_ENGINE::EngineApp::RegisterTimer(std::shared_ptr<StepTimer> timer)
{
	EngineResources::RegisterTimer(timer);
}

void LIME_ENGINE::EngineApp::GetDefaultDimensions(float & width, float & height)
{
	width = m_defaultDimensions.x;
	height = m_defaultDimensions.y;
}

void LIME_ENGINE::EngineApp::Tick()
{
	if (m_bActivated)
	{
		EngineResources::GetConsole()->Update();
		OnUpdate();
	}
}

void LIME_ENGINE::EngineApp::Render()
{
	if (m_bActivated)
	{
		auto graphics = EngineResources::GetGraphicsDevice();
		auto console = EngineResources::GetConsole();

		graphics->BeginScene(m_clearColor.x, m_clearColor.g, m_clearColor.b);
		OnRender();
		console->Render();
		graphics->EndScene();
	}
}

void LIME_ENGINE::EngineApp::UpdateDimensions()
{
	if (m_bActivated)
	{
		EngineResources::GetConsole()->UpdateDimensions();
		OnWindowSizeChanged();
	}
}

void LIME_ENGINE::EngineApp::Suspend()
{
	OnSuspend();
	EngineResources::Reset();
	m_bActivated = false;
}

void LIME_ENGINE::EngineApp::Resume()
{
	if (EngineResources::Initialize())
	{
		m_bActivated = true;
		OnResume();
	}
}

void LIME_ENGINE::EngineApp::OnWindowVisibilityChange()
{

}

void LIME_ENGINE::EngineApp::OnWindowSizeChanged()
{

}

void LIME_ENGINE::EngineApp::OnWindowMoved()
{
	
}

void LIME_ENGINE::EngineApp::SetClearColor(float r, float g, float b)
{
	m_clearColor.x = r;
	m_clearColor.y = g;
	m_clearColor.z = b;
}

void LIME_ENGINE::EngineApp::SetDefaultDimensions(float width, float height)
{
	m_defaultDimensions.x = width;
	m_defaultDimensions.y = height;
}
