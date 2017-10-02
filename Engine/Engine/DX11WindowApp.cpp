#include "DX11WindowApp.h"
#include "DirectXMath.h"
#include "Windowsx.h"



Lime::DX11WindowApp::DX11WindowApp(LPCWSTR AppName, std::unique_ptr<DX11App> app)
{
	m_appName = AppName;
	WNDCLASSEX wc = { 0 };
	m_hinstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_width = 800;
	m_height = 600;
	app.swap(m_application);
	app = nullptr;
}

int Lime::DX11WindowApp::Initialize()
{
	if (m_application == nullptr)
		return 1;

	if (!DirectX::XMVerifyCPUSupport())
		return 1;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	// Register class and create window
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_appName;
	wcex.hIconSm = NULL;
	if (!RegisterClassEx(&wcex))
		return 1;


	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = static_cast<LONG>(m_width);
	rc.bottom = static_cast<LONG>(m_height);
	m_application->SetSize(m_width, m_height);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(
		0, 
		m_appName, 
		m_appName, 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		rc.right - rc.left, rc.bottom - rc.top, 
		nullptr, 
		nullptr, 
		m_hinstance,
		nullptr);
	// TODO: Change to CreateWindowEx(WS_EX_TOPMOST, L"Direct3D_Win32_Game3WindowClass", L"Direct3D Win32 Game3", WS_POPUP,
	// to default to fullscreen.

	if (!m_hwnd)
		return 1;

	ShowWindow(m_hwnd, SW_SHOW);

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_application.get()));
	m_application->Init(m_hwnd);

	return 0;
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
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_application->Tick();
		}
	}

	m_application.reset();

	CoUninitialize();

	return (int)msg.wParam;
}

LRESULT Lime::WinProc::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool isCharUp = false;
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;
	// TODO: Set s_fullscreen to true if defaulting to fullscreen.

	auto app = reinterpret_cast<DX11App*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_MOVE:
		if (app)
		{
			app->OnWindowMoved();
		}
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend && app)
					app->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend && app)
				app->OnResuming();
			s_in_suspend = false;
		}
		else if (!s_in_sizemove && app)
		{
			app->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (app)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (app)
		{
			if (wParam)
			{
				app->OnActivated();
			}
			else
			{
				app->OnDeactivated();
			}
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && app)
				app->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend && app)
					app->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		app->KeyDown((unsigned int)wParam);
		break;

	case WM_KEYUP:
		app->KeyUp((unsigned int)wParam);
		break;

	case WM_CHAR:
		//Implement to check button pressed character
		break;

	case WM_LBUTTONDOWN:
		switch (wParam)
		{
		case MK_RBUTTON:
		case MK_MBUTTON:
		case MK_LBUTTON:
			app->KeyDown((unsigned int)wParam);
			break;
		default:
			break;
		}
		break;

	case WM_LBUTTONUP:
		switch (wParam)
		{
		case MK_RBUTTON:
		case MK_MBUTTON:
		case MK_LBUTTON:
			app->KeyUp((unsigned int)wParam);
			break;
		default:
			break;
		}
		break;

	case WM_MOUSEMOVE:
	{
		short xPos = GET_X_LPARAM(lParam);
		short yPos = GET_Y_LPARAM(lParam);
		app->SetMouseCoords(xPos, yPos);
	}
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

				UINT width = 800;
				UINT height = 600;
				if (app)
				{
					app->GetDefaultSize(width, height);
					RECT rc;
					rc.top = 0;
					rc.left = 0;
					rc.right = static_cast<LONG>(width);
					rc.bottom = static_cast<LONG>(height);
					AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
					width = rc.right - rc.left;
					height = rc.bottom - rc.top;
				}

				ShowWindow(hWnd, SW_SHOWNORMAL);

				SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, 0);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
