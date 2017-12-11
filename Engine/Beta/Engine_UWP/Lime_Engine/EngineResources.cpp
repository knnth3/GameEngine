#include "EngineResources.h"


std::shared_ptr<Graphics::GraphicsDevice> LIME_ENGINE::EngineResources::m_graphicsDevice = nullptr;
std::shared_ptr < LIME_ENGINE::InputManager > LIME_ENGINE::EngineResources::m_inputManager = nullptr;
std::shared_ptr<LIME_ENGINE::StepTimer> LIME_ENGINE::EngineResources::m_timer = nullptr;
std::shared_ptr<LIME_ENGINE::Console> LIME_ENGINE::EngineResources::m_console = nullptr;

bool LIME_ENGINE::EngineResources::Initialize()
{
	if (m_graphicsDevice && m_inputManager && m_timer)
	{
		m_console = std::shared_ptr<Console>(new Console());
		m_console->UpdateDimensions();
		return true;
	}
	return false;
}

void LIME_ENGINE::EngineResources::RegisterGraphicsDevice(std::shared_ptr<Graphics::GraphicsDevice> graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;
}

void LIME_ENGINE::EngineResources::RegisterInputManager(std::shared_ptr<InputManager> inputManager)
{
	m_inputManager = inputManager;
}

void LIME_ENGINE::EngineResources::RegisterTimer(std::shared_ptr<StepTimer> timer)
{
	m_timer = timer;
}

void LIME_ENGINE::EngineResources::Reset()
{
	m_graphicsDevice->Reset();
	m_graphicsDevice = nullptr;
	m_inputManager = nullptr;
	m_timer = nullptr;
}

std::shared_ptr<Graphics::GraphicsDevice> LIME_ENGINE::EngineResources::GetGraphicsDevice()
{
	return m_graphicsDevice;
}

std::shared_ptr<LIME_ENGINE::InputManager> LIME_ENGINE::EngineResources::GetInputManager()
{
	return m_inputManager;
}

std::shared_ptr<LIME_ENGINE::StepTimer> LIME_ENGINE::EngineResources::GetTimer()
{
	return m_timer;
}

std::shared_ptr<LIME_ENGINE::Console> LIME_ENGINE::EngineResources::GetConsole()
{
	return m_console;
}
