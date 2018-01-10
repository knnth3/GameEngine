#include "GraphicsWindow.h"
#include <future>

Graphics::GraphicsWindow* window = nullptr;

Graphics::GraphicsWindow::GraphicsWindow(const std::string WindowTitle):
	GraphicsWindow(To_wstr(WindowTitle))
{
}

Graphics::GraphicsWindow::GraphicsWindow(const std::wstring WindowTitle)
{
	window = this;
	m_windowTitle = WindowTitle;
	m_lastError = "";
	m_hwnd = NULL;
	m_hInstance = GetModuleHandle(NULL);
	m_bInitialized = false;
	m_brunning = false;
	m_size = { 800.0f, 600.0f };
}

Graphics::GraphicsWindow::~GraphicsWindow()
{
	m_windowTitle = L"";
	m_lastError = "";
	m_hwnd = NULL;
	m_hInstance = NULL;
	m_bInitialized = false;
	m_size = { 800.0f, 600.0f };
}

bool Graphics::GraphicsWindow::Initialize()
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

	m_hwnd = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600, NULL, NULL, m_hInstance, NULL);

	if (!m_hwnd)
	{
		m_lastError = "CreateWindow failed";
		return false;
	}

	m_bInitialized = InitGraphics();
	m_brunning = m_bInitialized;
	return m_bInitialized;
}

int Graphics::GraphicsWindow::Run()
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

std::string Graphics::GraphicsWindow::GetLastError()
{
	return m_lastError;
}

LRESULT Graphics::GraphicsWindow::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Graphics::GraphicsWindow::ProcessApp()
{
	while (m_brunning)
	{
		m_timer->update();
		Update();
		m_graphics->BeginScene();
		Render(m_graphics);
		m_graphics->EndScene();
		m_graphics->Present();
	}
	return true;
}

bool Graphics::GraphicsWindow::InitGraphics()
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

void Graphics::GraphicsWindow::ShutdownGraphics()
{
	WindowResources::Reset();
	m_input.reset();
	m_timer.reset();
	m_graphics.reset();
	CoUninitialize();
}

LRESULT Graphics::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(window)
		return window->WinProc(hwnd, uMsg, wParam, lParam);

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
