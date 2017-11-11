#pragma once
#include "Window.h"
#include <SDL2\SDL.h>


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
		bool m_opengl;
		bool m_initialized;
		std::string m_appName;
		std::unique_ptr<App> m_application;
		SDL_Window* m_window;
	};
}

