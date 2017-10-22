#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Shared.h"
#include "App.h"

#define WIN32_LEAN_AND_MEAN
#define FULL_SCREEN false

namespace Lime
{

	class Win32Window
	{
	public:
		AppDLL_API Win32Window(LPCWSTR AppName, std::unique_ptr<App>& app);
		AppDLL_API bool Initialize();
		AppDLL_API ~Win32Window();
		AppDLL_API int Run();
		AppDLL_API LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
		AppDLL_API bool IsAppInitialized();

	private:
		unsigned int m_width;
		unsigned int m_height;
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		std::unique_ptr<App> m_application;
	};


	static Win32Window* ApplicationHandle = nullptr;

	static LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

}