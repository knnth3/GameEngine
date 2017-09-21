#include "DX11WindowApp.h"



Lime::DX11WindowApp::DX11WindowApp(LPCWSTR AppName)
{
	m_appName = AppName;
	WNDCLASSEX wc = {0};
	DEVMODE dmScreenSettings = {0};
	m_hinstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_width = 800;
	m_height = 600;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	// Setup the windows class with default settings.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hinstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = m_appName;

	// Register the window class.
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		//memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		m_width = GetSystemMetrics(SM_CXSCREEN);
		m_height = GetSystemMetrics(SM_CYSCREEN);
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	// Create the window with the screen settings and get the handle to it.
	RECT wr = { 0, 0, (long)m_width, (long)m_height };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	m_hwnd = CreateWindowEx(NULL,
		m_appName,    // name of the window class
		m_appName,   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		m_hinstance,    // application handle
		NULL);    // used with multiple windows, NULL

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	return;
}

Lime::DX11WindowApp::~DX11WindowApp()
{
	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_appName, m_hinstance);
	m_hinstance = NULL;
}

bool Lime::DX11WindowApp::Run()
{
	MSG msg = {0};
	bool running = true;

	// Handle the windows messages.
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			running = false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return running;
}

const HWND Lime::DX11WindowApp::GetHWND()
{
	return m_hwnd;
}

unsigned int Lime::DX11WindowApp::GetWindowWidth() const
{
	return m_width;
}

unsigned int Lime::DX11WindowApp::GetWindowHeight() const
{
	return m_height;
}

LRESULT Lime::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
