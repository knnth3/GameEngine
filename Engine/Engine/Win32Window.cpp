#include "Win32Window.h"
#include "DirectXMath.h"
#include "Windowsx.h"

Lime::Win32Window::Win32Window(LPCWSTR AppName, std::unique_ptr<App>& app)
{
	m_appName = AppName;
	WNDCLASSEX wc = { 0 };
	m_hinstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	app.swap(m_application);
	app = nullptr;
}

bool Lime::Win32Window::Initialize()
{
	if (m_application == nullptr)
		return false;

	if (!DirectX::XMVerifyCPUSupport())
		return false;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return false;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);
	//Gets a global ptr so it can be refrenced by WindowProc
	ApplicationHandle = this;

	WNDCLASSEX wc = { 0 };
	DEVMODE dmScreenSettings;
	int posX, posY;
	bool result;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = Lime::WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	if (m_application->GetAPI() == Lime::GRAPHICS_API::OPENGL_4_0)
	{
		bool result;
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName, WS_POPUP,
			0, 0, 640, 480, NULL, NULL, m_hinstance, NULL);
		if (m_hwnd == NULL)
		{
			return false;
		}

		// Don't show the window.
		ShowWindow(m_hwnd, SW_HIDE);

		// Initialize a temporary OpenGL window and load the OpenGL extensions.
		result = m_application->InitializeGraphics(m_hwnd);
		if (!result)
		{
			MessageBox(m_hwnd, L"Could not initialize the OpenGL extensions.", L"Error", MB_OK);
			return false;
		}

		// Release the temporary window now that the extensions have been initialized.
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

	}
	else
	{
		result = m_application->InitializeGraphics(m_hwnd);
		if (!result)
		{
			MessageBox(m_hwnd, L"Could not initialize DirectX.", L"Error", MB_OK);
			return false;
		}
	}

	// Determine the resolution of the clients desktop screen.
	m_width = GetSystemMetrics(SM_CXSCREEN);
	m_height = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		m_application->SetSize(m_width, m_height);
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		m_width = 800;
		m_height = 600;
		m_application->SetSize(m_width, m_height);

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName, WS_OVERLAPPEDWINDOW,
		posX, posY, m_width, m_height, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	result = m_application->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize App", L"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_application.get()));

	return result;
}

Lime::Win32Window::~Win32Window()
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

int Lime::Win32Window::Run()
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
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

//Should only run if m_application is initialized
LRESULT Lime::Win32Window::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool isCharUp = false;
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;
	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_MOVE:
		m_application->OnWindowMoved();
		break;

	case WM_SIZE:
		if (wparam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend)
					m_application->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend)
				m_application->OnResuming();
			s_in_suspend = false;
		}
		else if (!s_in_sizemove)
		{
			m_application->OnWindowSizeChanged(LOWORD(lparam), HIWORD(lparam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		RECT rc;
		GetClientRect(hwnd, &rc);
		m_application->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lparam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (wparam)
		{
			m_application->OnActivated();
		}
		else
		{
			m_application->OnDeactivated();
		}
		break;

	case WM_POWERBROADCAST:
		switch (wparam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend)
				m_application->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend)
					m_application->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		m_application->KeyDown((unsigned int)wparam);
		break;

	case WM_KEYUP:
		m_application->KeyUp((unsigned int)wparam);
		break;

	case WM_CHAR:
		//Implement to check button pressed character
		break;

	case WM_LBUTTONDOWN:
		m_application->KeyDown(0x01);
		break;

	case WM_LBUTTONUP:
		m_application->KeyUp(0x01);
		break;

	case WM_MOUSEMOVE:
	{
		short xPos = GET_X_LPARAM(lparam);
		short yPos = GET_Y_LPARAM(lparam);
		m_application->SetMouseCoords(xPos, yPos);
	}
	break;

	case WM_SYSKEYDOWN:
		if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

				uint32_t width = 800;
				uint32_t height = 600;
				m_application->GetWindowSize(width, height);
				RECT rc;
				rc.top = 0;
				rc.left = 0;
				rc.right = static_cast<LONG>(width);
				rc.bottom = static_cast<LONG>(height);
				AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
				width = rc.right - rc.left;
				height = rc.bottom - rc.top;

				ShowWindow(hwnd, SW_SHOWNORMAL);

				SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(hwnd, GWL_STYLE, 0);
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);
	}

	return 0;
}

bool Lime::Win32Window::IsAppInitialized()
{
	if (m_application)
		return m_application->IsInitialized();
	else
		return false;
}

LRESULT Lime::WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	// All other messages pass to the message handler in the system class.
	default:
		if(ApplicationHandle->IsAppInitialized())
			ApplicationHandle->MessageHandler(hwnd, message, wParam, lParam);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
