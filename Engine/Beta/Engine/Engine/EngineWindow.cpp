#include "EngineWindow.h"
#include <future>

#define WS_NORESIZEWINDOW WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX

Engine::GraphicsWindow* window = nullptr;
WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

Engine::GraphicsWindow::GraphicsWindow(const std::string WindowTitle, float width, float height, bool fullscreen):
	GraphicsWindow(To_wstr(WindowTitle), width, height, fullscreen)
{
}

Engine::GraphicsWindow::GraphicsWindow(const std::wstring WindowTitle, float width, float height, bool fullscreen)
{
	window = this;
	m_windowTitle = WindowTitle;
	m_lastError = "";
	m_hwnd = NULL;
	m_hInstance = GetModuleHandle(NULL);
	m_bInitialized = false;
	m_brunning = false;
	m_bFullscreen = fullscreen;
	m_size = { width, height };
	m_bResize = false;
}

Engine::GraphicsWindow::~GraphicsWindow()
{
	m_windowTitle = L"";
	m_lastError = "";
	m_hwnd = NULL;
	m_hInstance = NULL;
	m_bInitialized = false;
	m_size = { 800.0f, 600.0f };
}

bool Engine::GraphicsWindow::Initialize()
{
	LPCWSTR title = m_windowTitle.c_str();
	WNDCLASSEX wcexDesc = {};
	wcexDesc.cbSize = sizeof(WNDCLASSEX);
	wcexDesc.style = CS_HREDRAW | CS_VREDRAW;
	wcexDesc.lpfnWndProc = WndProc;
	wcexDesc.cbClsExtra = 0;
	wcexDesc.hInstance = m_hInstance;
	wcexDesc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wcexDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexDesc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcexDesc.lpszMenuName = NULL;
	wcexDesc.lpszClassName = title;
	wcexDesc.hIconSm = LoadIcon(wcexDesc.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcexDesc))
	{
		m_lastError = "RegisterClassEx failed";
		return false;
	}

	RECT rect = { 0 };
	rect.right = (LONG)roundf(m_size.Width);
	rect.bottom = (LONG)roundf(m_size.Height);
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	m_hwnd = CreateWindow(title, title, WS_NORESIZEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, NULL);

	if (!m_hwnd)
	{
		m_lastError = "CreateWindow failed";
		return false;
	}

	m_bInitialized = InitGraphics();
	m_brunning = m_bInitialized;
	return m_bInitialized;
}

int Engine::GraphicsWindow::Run()
{
	MSG msg = {};
	msg.wParam = -1;
	if (m_bInitialized)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
		Resume();
		std::future<bool> result(std::async(std::launch::async, &GraphicsWindow::ProcessApp, this));
		do
		{
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} while (msg.message != WM_QUIT);
		m_brunning = false;
		result.wait();
		m_graphics->Trim();
		Suspend();
		ShutdownGraphics();
	}
	return (int)msg.wParam;
}

std::string Engine::GraphicsWindow::GetLastError()
{
	return m_lastError;
}

LRESULT Engine::GraphicsWindow::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto input = WindowResources::GetInput();

	switch (uMsg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		if (input)
		{
			auto mouse = input->GetMouse();
			if (mouse)
			{
				POINTS p = MAKEPOINTS(lParam);
				mouse->SetPosition(p.x, p.y);
			}
		}
		break;

	case WM_KEYDOWN:
		if (input)
		{
			auto keyboard = input->GetKeyboard();
			if (keyboard)
			{
				keyboard->SetButtonStatus((int)wParam, true);
			}
		}
		break;

	case WM_KEYUP:
		if (input)
		{
			auto keyboard = input->GetKeyboard();
			if (keyboard)
			{
				keyboard->SetButtonStatus((int)wParam, false);
			}
		}
		break;

	case WM_SIZE:
		m_size.Width = LOWORD(lParam); // width of client area
		m_size.Height = HIWORD(lParam); // height of client area
		m_bResize = true;
		break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Engine::GraphicsWindow::SetWindowWidth(float width)
{
	if (m_size.Width != width)
	{
		m_size.Width = width;
		ResizeWindowBySettings();
	}
}

void Engine::GraphicsWindow::SetWindowHeight(float height)
{
	if (m_size.Height != height)
	{
		m_size.Height = height;
		ResizeWindowBySettings();
	}
}

void Engine::GraphicsWindow::SetDisplaySize(DisplaySize size)
{
	if (m_size != size)
	{
		m_size = size;
		ResizeWindowBySettings();
	}
}

void Engine::GraphicsWindow::ToggleFullscreen()
{
	DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW) {
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(m_hwnd, &g_wpPrev) &&
			GetMonitorInfo(MonitorFromWindow(m_hwnd,
				MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLong(m_hwnd, GWL_STYLE,
				dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(m_hwnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_ASYNCWINDOWPOS);
		}
	}
	else {
		SetWindowLong(m_hwnd, GWL_STYLE,
			dwStyle | WS_NORESIZEWINDOW);
		SetWindowPlacement(m_hwnd, &g_wpPrev);
		SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_ASYNCWINDOWPOS);
	}
}

bool Engine::GraphicsWindow::ProcessApp()
{
	while (m_brunning)
	{
		if (m_bResize)
		{
			m_graphics->SetWindowDimensions(m_size);
			m_bResize = false;
		}
		m_timer->update();
		Update();
		m_graphics->BeginScene();
		Render(m_graphics);
		m_graphics->EndScene();
		m_graphics->Present();
	}
	return true;
}

bool Engine::GraphicsWindow::InitGraphics()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		return false;

	m_graphics = std::make_shared<GraphicsDevice>();
	m_graphics->Initialize(m_hwnd, m_size);

	m_timer = std::make_shared<Timer>();
	m_input = std::make_shared<Input>();

	universal_struct input(m_input);
	universal_struct timer(m_timer);
	universal_struct graphics(m_graphics);
	WindowResources::SetGraphics(graphics);
	WindowResources::SetTimer(timer);
	WindowResources::SetInput(input);
	return true;
}

void Engine::GraphicsWindow::ShutdownGraphics()
{
	WindowResources::Reset();
	m_input.reset();
	m_timer.reset();
	m_graphics.reset();
	if (m_bFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	CoUninitialize();
}

void Engine::GraphicsWindow::ResizeWindowBySettings(bool move, glm::vec2 pos)
{
	RECT rect = { 0 };
	rect.left = (LONG)pos.x;
	rect.top = (LONG)pos.y;
	rect.right = (LONG)roundf(rect.left + m_size.Width);
	rect.bottom = (LONG)roundf(rect.top + m_size.Height);
	::AdjustWindowRect(&rect, WS_NORESIZEWINDOW, false);
	if (move)
		SetWindowPos(m_hwnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
	else
		SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
}

LRESULT Engine::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(window)
		return window->WinProc(hwnd, uMsg, wParam, lParam);

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}