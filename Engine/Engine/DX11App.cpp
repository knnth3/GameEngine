#include "DX11App.h"



Lime::DX11App::DX11App()
{
}


Lime::DX11App::~DX11App()
{
}

void Lime::DX11App::Init(HWND wnd)
{
	m_graphicsDevice = std::make_unique<DX11Graphics>(wnd, m_width, m_height);
	m_input = std::make_unique<InputManager>();
	Initialize();
}

void Lime::DX11App::CloseApp()
{
	PostQuitMessage(0);
}

void Lime::DX11App::GetDefaultSize(UINT& width, UINT& height)
{
	width = m_width;
	height = m_height;
}

void Lime::DX11App::SetSize(UINT width, UINT height)
{
	m_width = width;
	m_height = height;

}

void Lime::DX11App::KeyUp(unsigned int Key)
{
	m_input->KeyUp(Key);
}

void Lime::DX11App::KeyDown(unsigned int Key)
{
	m_input->KeyDown(Key);
}

void Lime::DX11App::SetMouseCoords(short x, short y)
{
	m_input->SetMouseCoords(x, y);
}

void Lime::DX11App::SetWindowSize(UINT width, UINT height)
{
	m_width = width;
	m_height = height;
}
