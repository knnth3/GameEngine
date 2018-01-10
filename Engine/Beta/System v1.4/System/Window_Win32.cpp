#include "Window_Win32.h"
#include "Windowsx.h"


System::Window_Win32::Window_Win32(const std::string& appName, std::unique_ptr<App>& app, bool fullscreen)
{
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_bInitialized = false;
	m_bFullscreen = fullscreen;
	m_application = std::unique_ptr<App>(app.release());
	m_application->GetWindowSize(m_windowWidth, m_windowHeight);
	m_appName = StringToWideString(appName);

	//Define a Window Class
	m_windowClassDesc = { 0 };
	m_windowClassDesc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	m_windowClassDesc.lpfnWndProc = System::WindowProc;
	m_windowClassDesc.cbClsExtra = 0;
	m_windowClassDesc.cbWndExtra = 0;
	m_windowClassDesc.hInstance = m_hinstance;
	m_windowClassDesc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_windowClassDesc.hIconSm = m_windowClassDesc.hIcon;
	m_windowClassDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_windowClassDesc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_windowClassDesc.lpszMenuName = NULL;
	m_windowClassDesc.lpszClassName = m_appName.c_str();
	m_windowClassDesc.cbSize = sizeof(WNDCLASSEX);
}


System::Window_Win32::~Window_Win32()
{
	// Fix the display settings if leaving full screen mode.
	if (m_bFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	if (m_bInitialized)
	{
		// Remove the window.
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		// Remove the application instance.
		UnregisterClass(m_appName.c_str(), m_hinstance);
		m_hinstance = NULL;
	}
}

bool System::Window_Win32::Initialize()
{
	//Make sure the application is valid
	if (m_application == nullptr)
	{
		MessageBox(m_hwnd, L"App was nullptr", L"User Error", MB_OK);
		return false;
	}

	//CoInitialize for DirectX
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, L"CoInitializeEx failed" , L"System Error", MB_OK);
		return false;
	}

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);
	//Gets a global ptr so it can be refrenced by WindowProc
	ApplicationHandle = this;

	DEVMODE dmScreenSettings;
	int posX, posY;
	int maxX, maxY;

	// Register the window class.
	RegisterClassEx(&m_windowClassDesc);

	// Determine the resolution of the clients desktop screen.
	maxX = GetSystemMetrics(SM_CXSCREEN);
	maxY = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_bFullscreen)
	{
		m_application->SetWindowSize(maxX, maxY);
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)maxX;
		dmScreenSettings.dmPelsHeight = (unsigned long)maxY;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it the app'str settings
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_windowWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_windowHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName.c_str(), m_appName.c_str(), WS_OVERLAPPEDWINDOW,
		posX, posY, m_windowWidth, m_windowHeight, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		MessageBox(m_hwnd, L"Could not initialize Window", L"System Error", MB_OK);
		return false;
	}
	m_bInitialized = true;
	auto m_graphics = std::make_shared<Graphics::GraphicsDevice>(m_application->GetCamera());
	if (m_graphics->Initialize(m_hwnd, m_windowWidth, m_windowHeight, false, false))
		m_application->Initialize(m_graphics);
	else
	{
		MessageBox(m_hwnd, L"Could not initialize DirectX", L"Graphics Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	m_application->OnInitialize();

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_application.get()));

	return m_bInitialized;
}

int System::Window_Win32::Run()
{
	MSG message = {};
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			m_application->Tick();
		}
	}

	CoUninitialize();
	return m_application->GetOnExitCode();
}

bool System::Window_Win32::IsInitialized()
{
	return m_bInitialized;
}

LRESULT System::Window_Win32::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool isCharUp = false;
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_MOVE:
		m_application->OnWindowMoved();
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
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
			m_application->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
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
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (wParam)
		{
			m_application->OnActivated();
		}
		else
		{
			m_application->OnDeactivated();
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
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
		m_application->GetInputManager()->SetKeyDown((int)wParam);
		break;

	case WM_KEYUP:
		m_application->GetInputManager()->SetKeyUp((int) wParam);
		break;

	case WM_CHAR:
		//Implement to check button pressed character
		break;

	case WM_LBUTTONDOWN:
		m_application->GetInputManager()->SetKeyDown(1);
		break;

	case WM_LBUTTONUP:
		m_application->GetInputManager()->SetKeyUp(1);
		break;

	case WM_MOUSEMOVE:
	{
		short xPos = GET_X_LPARAM(lParam);
		short yPos = GET_Y_LPARAM(lParam);
		m_application->GetInputManager()->SetMouseCoords
		((uint16_t)xPos, (uint16_t)yPos);
	}
	break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

				uint16_t width = 800;
				uint16_t height = 600;
				m_application->GetWindowSize(width, height);
				RECT rc;
				rc.top = 0;
				rc.left = 0;
				rc.right = static_cast<LONG>(width);
				rc.bottom = static_cast<LONG>(height);
				AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
				width = uint16_t(rc.right - rc.left);
				height = uint16_t(rc.bottom - rc.top);

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

std::wstring System::Window_Win32::StringToWideString(const std::string str)
{
	// room for 100 characters
	std::vector<WCHAR> buffer(100);
	MultiByteToWideChar(
		CP_ACP,               // code page
		MB_PRECOMPOSED,       // character-type options
		str.c_str(),          // string to map
		(int)str.length(),         // number of bytes in string
		&buffer[0],           // wide-character buffer (must use vector here!)
		100                   // size of buffer
	);
	return std::wstring(buffer.begin(), buffer.end());
}

LRESULT System::WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		// All other messages pass to the message handler in the system class.
	default:
		if (ApplicationHandle->IsInitialized())
			ApplicationHandle->MessageHandler(hwnd, message, wParam, lParam);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
