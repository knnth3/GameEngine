#include "App.h"

#if PLATFORM == OS_WINDOWS

#include "DX11Graphics.h"
#include "OpenGLGraphics.h"

#else

#include "OpenGLGraphics.h"
#endif





Lime::App::App(const GRAPHICS_API api)
{
#if PLATFORM == OS_WINDOWS
	m_type = api;
#else
	m_type = GRAPHICS_API::OPENGL_4_0;
#endif

	m_width = 0;
	m_height = 0;
}

Lime::App::~App()
{
}

bool Lime::App::InitializeGraphics(void * hwnd)
{
#if PLATFORM == OS_WINDOWS
	if (m_type == Lime::GRAPHICS_API::DIRECTX_11)
		m_renderBatch = std::make_unique<DX11Graphics>();
	else if (m_type == Lime::GRAPHICS_API::OPENGL_4_0)
		m_renderBatch = std::make_unique<OpenGLGraphics>();
#else
	m_renderBatch = std::make_unique<OpenGLGraphics>();

#endif

	return m_renderBatch->PreInitialize(hwnd);
}

bool Lime::App::Initialize(void* hwnd)
{
	bool result;
	result = m_renderBatch->Initialize(hwnd, m_width, m_height);
	if (result)
	{
		m_input = std::make_unique<InputManager>();
		OnInitialize();
		m_isInitialized =  true;
	}
	return result;
}

void Lime::App::CloseApp()
{
	PostQuitMessage(0);
}

void Lime::App::GetWindowSize(uint32_t& width, uint32_t& height)
{
	width = m_width;
	height = m_height;
}

void Lime::App::SetSize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;

}

Lime::GRAPHICS_API Lime::App::GetAPI()
{
	return m_type;
}

void Lime::App::KeyUp(unsigned int Key)
{
	m_input->KeyUp(Key);
}

void Lime::App::KeyDown(unsigned int Key)
{
	m_input->KeyDown(Key);
}

void Lime::App::SetMouseCoords(short x, short y)
{
	m_input->SetMouseCoords(x, y);
}

void Lime::App::SetWindowSize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
}

bool Lime::App::IsInitialized()
{
	return m_isInitialized;
}
