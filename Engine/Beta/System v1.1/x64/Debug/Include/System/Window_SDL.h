#pragma once
#include "Window.h"
#include "ThreadSafeQueue.h"
#include <SDL2\SDL.h>
#include <future>
#include <atomic>


namespace System
{
	class Window_SDL :
		public Window
	{
	public:
		Window_SDL(const std::string& appName, std::unique_ptr<App>& app, bool opengl);
		~Window_SDL();

		//Overloaded Functions
		virtual bool Initialize() override;
		virtual int Run() override;

	private:
		int AppThreadFunc(std::unique_ptr<App>& app);

		bool m_opengl;
		bool m_initialized;
		std::future<int> m_future;
		std::atomic_bool m_bRunning;
		std::atomic_char16_t m_xResolution;
		std::atomic_char16_t m_yResolution;
		SafeQueue<SDL_Event> m_queue;
		std::string m_appName;
		std::unique_ptr<App> m_application;
		SDL_Window* m_window;
	};
}

