#include "Window_SDL.h"
#include <SDL2\SDL_syswm.h>

System::Window_SDL::Window_SDL(const std::string & appName, std::unique_ptr<App>& app, bool opengl)
{
	m_appName = appName;
	m_application = std::unique_ptr<App>(app.release());
	m_initialized = false;
	m_bRunning = true;
	m_opengl = opengl;
	m_xResolution = 800;
	m_yResolution = 600;
}

System::Window_SDL::~Window_SDL()
{
	if(m_window)
		SDL_DestroyWindow(m_window);
	if (m_initialized)
		SDL_Quit();
}

bool System::Window_SDL::Initialize()
{
	SDL_SysWMinfo info;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
#ifdef _DEBUG
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
#endif
		return false;
	}
	m_initialized = true;
	uint16_t width = 800;
	uint16_t height = 600;
	m_application->GetWindowSize(width, height);
	UINT flags = NULL;
	if (m_opengl)
	{
		flags |= SDL_WINDOW_OPENGL;
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	m_window = SDL_CreateWindow(m_appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)width, (int)height, flags);
	if (m_window)
	{
		SDL_GetVersion(&info.version);

		/* the call returns true on success */
		if (SDL_GetWindowWMInfo(m_window, &info))
		{
			const char *subsystem = "an unknown system!";
			std::shared_ptr<Graphics::GraphicsDevice> m_graphics;
			switch (info.subsystem)
			{
			case SDL_SYSWM_UNKNOWN:   break;
			case SDL_SYSWM_WINDOWS:
				subsystem = "Microsoft Windows(TM)";

				//init graphics
				m_graphics = std::make_shared<Graphics::GraphicsDevice>(m_application->GetCamera());
				if(m_graphics->Initialize(info.info.win.window, width, height, false, false))
					m_application->Initialize(m_graphics);
				break;
			case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
			case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
			case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
			case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
			case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
			case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
			case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
			case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
			case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
#endif
			}
#ifdef _DEBUG
			SDL_Log("This program is running SDL version %d.%d.%d on %s",
				(int)info.version.major,
				(int)info.version.minor,
				(int)info.version.patch,
				subsystem);
#endif
		}
		else
		{
#ifdef _DEBUG
			/* call failed */
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't get window information: %s", SDL_GetError());
#endif
		}
		m_application->OnInitialize();
		auto VideoDrivers = SDL_GetVideoDriver(0);
		std::string result = VideoDrivers;
	}
	
	return true;
}

int System::Window_SDL::Run()
{
	//Do not modify m_window here!
	auto func = std::bind(&Window_SDL::AppThreadFunc, this, std::placeholders::_1);
	m_future = std::async(std::launch::async, func, std::ref(m_application));
	SDL_Event e;
	int x_res, y_res;
	do
	{
		//Processes events until none left or SDL_QUIT
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				m_bRunning = false;
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_RESTORED:
					SDL_GetWindowSize(m_window, &x_res, &y_res);
					m_xResolution = x_res;
					m_yResolution = y_res;
				case SDL_WINDOWEVENT_MINIMIZED:
					break;
				}
			case SDL_KEYUP:
			case SDL_KEYDOWN:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEWHEEL:
				m_queue.push(e);
				break;
			default:
				break;
			}
		}

	} while (m_bRunning);
	return m_future.get();
}

int System::Window_SDL::AppThreadFunc(std::unique_ptr<App>& app)
{
	SDL_Event e;
	auto myApp = std::unique_ptr<App>(app.release());
	bool bIsSuspended = false;
	while (m_bRunning)
	{
		if (myApp->IsClosing())
		{
			m_bRunning = false;
			break;
		}
		if (m_queue.front(e))
		{
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
					if (bIsSuspended)
					{
						myApp->OnResuming();
						bIsSuspended = false;
					}
				case SDL_WINDOWEVENT_RESIZED:
					myApp->SetWindowSize(m_xResolution, m_yResolution);
					myApp->OnWindowSizeChanged(m_xResolution, m_yResolution);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					if (!bIsSuspended)
					{
						myApp->OnSuspending();
						bIsSuspended = true;
					}
					break;
				}
				break;
			case SDL_KEYUP:
				myApp->GetInputManager()->SetKeyUp((int)e.key.keysym.sym);
				break;
			case SDL_KEYDOWN:
				myApp->GetInputManager()->SetKeyDown((int)e.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				myApp->GetInputManager()->SetMouseCoords((uint16_t)e.motion.x, (uint16_t)e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				myApp->GetInputManager()->SetKeyDown((int)e.button.button);
				break;
			case SDL_MOUSEBUTTONDOWN:
				myApp->GetInputManager()->SetKeyDown((int)e.button.button);
				break;
			default:
				break;
			}
		}

		myApp->Tick();
	}
	return myApp->GetOnExitCode();
}
