#include "App.h"


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

void Lime::App::Init(void* hwnd)
{

#if PLATFORM == OS_WINDOWS
	HWND m_hwnd = (HWND)hwnd;
	if(m_type == Lime::GRAPHICS_API::DIRECTX_11)
		m_renderBatch = std::make_unique<DX11Graphics>(m_hwnd, m_width, m_height);
	else
		m_renderBatch = std::make_unique<DX11Graphics>(m_hwnd, m_width, m_height);
#else
	m_renderBatch = std::make_unique<DX11Graphics>(m_hwnd, m_width, m_height);
#endif

	m_input = std::make_unique<InputManager>();
	OnInitialize();
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