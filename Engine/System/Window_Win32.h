#pragma once
#include <memory>
#include "App.h"

//Macros
#define WIN32_LEAN_AND_MEAN
#define FULL_SCREEN false
using namespace std;

namespace System
{
	class Window_Win32
	{
	public:
		Window_Win32(LPCWSTR AppName, unique_ptr<App>& app);
		bool Initialize();
		~Window_Win32();
		int Run();
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	private:
		unsigned int m_width;
		unsigned int m_height;
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		WNDCLASSEX m_wc;
		unique_ptr<App> m_application;
	};


	static Window_Win32* ApplicationHandle = nullptr;

	static LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);
}