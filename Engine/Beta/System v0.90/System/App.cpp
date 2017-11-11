#include "App.h"


System::App::App(uint16_t windowWidth, uint16_t windowHeight)
{
	SetWindowSize(windowWidth, windowHeight);
	SetDefaultWindowSize(windowWidth, windowHeight);
	m_bIsClosing = false;
	m_onExitCode = 0;
}

System::App::~App()
{
	m_graphics->Close();
}

void System::App::CloseApp(int code)
{
	m_onExitCode = code;
	m_bIsClosing = true;
}

void System::App::GetWindowSize(uint16_t& width, uint16_t& height)
{
	width  = m_width;
	height = m_height;
}

void System::App::SetWindowSize(uint16_t  width, uint16_t  height)
{
	m_width  = width;
	m_height = height;
}

void System::App::GetDefaultWindowSize(uint16_t & width, uint16_t & height)
{
	width  = m_defaultWidth;
	height = m_defaultHeight;
}

void System::App::SetDefaultWindowSize(uint16_t width, uint16_t height)
{
	m_defaultWidth  = width;
	m_defaultHeight = height;
}

bool System::App::IsClosing()
{
	return m_bIsClosing;
}

void System::App::Initialize(std::shared_ptr<Graphics::Camera>& camera, 
	std::shared_ptr<Graphics::GraphicsDevice>& graphics)
{
	m_camera = camera;
	m_graphics = graphics;
}

std::shared_ptr<Graphics::Camera> System::App::GetCamera()
{
	return m_camera;
}

std::shared_ptr<Graphics::GraphicsDevice> System::App::GetGraphicsDevice()
{
	return m_graphics;
}

int System::App::GetOnExitCode()
{
	return m_onExitCode;
}

void System::App::Tick()
{
}

void System::App::OnInitialize()
{
}

void System::App::OnActivated()
{
}

void System::App::OnDeactivated()
{
}

void System::App::OnSuspending()
{
}

void System::App::OnResuming()
{
}

void System::App::OnWindowMoved()
{
}

void System::App::OnWindowSizeChanged(int width, int height)
{
}
