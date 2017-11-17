#include "EngineApp.h"

using namespace LIME_ENGINE;
LIME_ENGINE::EngineApp::EngineApp(uint16_t windowWidth, uint16_t windowHeight, std::string programPath):
	App(windowWidth, windowHeight, programPath)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

LIME_ENGINE::EngineApp::~EngineApp()
{
}

void LIME_ENGINE::EngineApp::OnInitialize()
{
	this->GetGraphicsDevice()->DrawBackToFront(true);
	m_console = std::make_shared<Console>(m_windowWidth);
	OnStartUp();
}

void LIME_ENGINE::EngineApp::Tick()
{
	if (this->GetInputManager()->KeyPressed(System::KEY_BACKQUOTE))
		m_console->Switch();
	m_console->RenderConsole(this->GetGraphicsDevice());
	OnTick();
}

void LIME_ENGINE::EngineApp::OnTick()
{

}

void LIME_ENGINE::EngineApp::OnStartUp()
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

void LIME_ENGINE::EngineApp::OnWindowSizeChanged(int width, int height)
{
}

std::shared_ptr<Console> LIME_ENGINE::EngineApp::GetConsole()
{
	return m_console;
}
