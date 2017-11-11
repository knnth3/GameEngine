#pragma once
#include "Window.h"
#include <Windows.h>

namespace System
{
	class Window_Win32:
		public Window
	{
	public:
		Window_Win32(const std::string& appName, std::unique_ptr<App>& app, bool fullscreen = false);
		~Window_Win32();

		//Overloaded Functions
		virtual bool Initialize() override;
		virtual int Run() override;

		bool IsInitialized();
		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		std::wstring StringToWideString(const std::string str);

		bool m_bFullscreen;
		bool m_bInitialized;
		uint16_t m_windowWidth;
		uint16_t m_windowHeight;
		std::wstring m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		WNDCLASSEX m_windowClassDesc;
		std::unique_ptr<App> m_application;
	};


	static Window_Win32* ApplicationHandle = nullptr;

	static LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

}