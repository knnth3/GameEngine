#include "App.h"


Lime::App::App()
{
	m_width = 0;
	m_height = 0;
}

Lime::App::~App()
{
}

void Lime::App::Init(HWND hwnd)
{
	m_hwnd = hwnd;
	m_graphicsDevice = std::make_unique<DX11Graphics>(m_hwnd, m_width, m_height);
	m_input = std::make_unique<InputManager>();
	OnInitialize();
}

void Lime::App::CloseApp()
{
	PostQuitMessage(0);
}

void Lime::App::GetDefaultSize(UINT& width, UINT& height)
{
	width = m_width;
	height = m_height;
}

void Lime::App::SetSize(UINT width, UINT height)
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

void Lime::App::SetWindowSize(UINT width, UINT height)
{
	m_width = width;
	m_height = height;
}