#include "App.h"


System::App::App()
{
	m_width = 0;
	m_height = 0;
	m_handle = nullptr;
}

void System::App::CloseApp()
{
	PostQuitMessage(0);
}

void System::App::GetWindowSize(uint32_t& width, uint32_t& height)
{
	width = m_width;
	height = m_height;
}

void System::App::SetWindowSize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
}

void * System::App::GetHandle()
{
	return m_handle;
}

void System::App::SetHandle(void * handle)
{
	m_handle = handle;
}
