#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Shared.h"
#include "App.h"

#define WIN32_LEAN_AND_MEAN
#define FULL_SCREEN false

namespace Lime
{
	class WinProc
	{
	public:
		AppDLL_API static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	class Win32Window
	{
	public:
		AppDLL_API Win32Window(LPCWSTR AppName, std::unique_ptr<App>& app);
		AppDLL_API int Initialize();
		AppDLL_API ~Win32Window();
		//Returns false on close
		AppDLL_API int Run();
	private:
		unsigned int m_width;
		unsigned int m_height;
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		std::unique_ptr<App> m_application;
	};

}