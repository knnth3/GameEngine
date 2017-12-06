#include "EngineApp.h"

using namespace Graphics;

LIME_ENGINE::EngineApp::EngineApp(std::shared_ptr<GraphicsDevice>& graphicsDevice, 
	std::shared_ptr<InputManager>& inputManager,
	std::shared_ptr<StepTimer>& timer)
{
	m_timer = timer;
	m_graphicsDevice = graphicsDevice;
	m_inputManager = inputManager;
	m_clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
	m_consoleCode = '`';
}

void LIME_ENGINE::EngineApp::Initialize()
{
	m_console = std::shared_ptr<Console>(new Console(m_graphicsDevice));
	CreateCommonBrushes();
	OnInitialize();
}

void LIME_ENGINE::EngineApp::Tick()
{
	if (m_inputManager->KeyPressed(m_consoleCode))
		m_console->Switch();

	m_console->Update();
	OnUpdate();
}

void LIME_ENGINE::EngineApp::Render()
{
	m_graphicsDevice->BeginScene(m_clearColor.x, m_clearColor.g, m_clearColor.b);
	this->OnRender();
	m_console->Render();
	m_graphicsDevice->EndScene();
}

void LIME_ENGINE::EngineApp::SetClearColor(float r, float g, float b)
{
	m_clearColor.x = r;
	m_clearColor.y = g;
	m_clearColor.z = b;
}

void LIME_ENGINE::EngineApp::SetDimensions(float width, float height)
{
	m_console->UpdateDimensions(width, height);
	this->OnWindowSizeChanged(width, height);
}

void LIME_ENGINE::EngineApp::OnUpdate()
{

}

void LIME_ENGINE::EngineApp::OnRender()
{
}

void LIME_ENGINE::EngineApp::OnInitialize()
{
}

void LIME_ENGINE::EngineApp::OnActivated()
{
}

void LIME_ENGINE::EngineApp::OnDeactivated()
{
}

void LIME_ENGINE::EngineApp::OnSuspending()
{
}

void LIME_ENGINE::EngineApp::OnResuming()
{
}

void LIME_ENGINE::EngineApp::OnWindowMoved()
{
}

void LIME_ENGINE::EngineApp::OnWindowSizeChanged(float width, float height)
{
}

void LIME_ENGINE::EngineApp::OnShutdown()
{
}

void LIME_ENGINE::EngineApp::SetConsoleOpenButton(int code)
{
	m_consoleCode = code;
}

std::shared_ptr<LIME_ENGINE::Console> LIME_ENGINE::EngineApp::GetConsole()
{
	return m_console;
}

std::shared_ptr<Graphics::GraphicsDevice> LIME_ENGINE::EngineApp::GetGraphicsDevice()
{
	return m_graphicsDevice;
}

std::shared_ptr<InputManager> LIME_ENGINE::EngineApp::GetInputManager()
{
	return m_inputManager;
}

std::shared_ptr<LIME_ENGINE::StepTimer> LIME_ENGINE::EngineApp::GetTimer()
{
	return m_timer;
}

void LIME_ENGINE::EngineApp::CreateCommonBrushes()
{
	m_graphicsDevice->CreateNew2DBrush("Red", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_graphicsDevice->CreateNew2DBrush("Green", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_graphicsDevice->CreateNew2DBrush("Blue", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	m_graphicsDevice->CreateNew2DBrush("Blue", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
