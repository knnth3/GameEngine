#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Primitives.h"
#include "DX11App.h"

#define WIN32_LEAN_AND_MEAN
#define FULL_SCREEN false

namespace Lime
{
	DLL_EXPORT LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Indicates to hybrid graphics systems to prefer the discrete part by default
	extern "C"
	{
		__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	}

	class DX11WindowApp
	{
	public:
		DLL_EXPORT DX11WindowApp(LPCWSTR AppName, std::unique_ptr<DX11App> app);
		DLL_EXPORT int Initialize();
		DLL_EXPORT ~DX11WindowApp();
		//Returns false on close
		DLL_EXPORT bool Run();
	private:
		unsigned int m_width;
		unsigned int m_height;
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		std::unique_ptr<DX11App> m_application;
	};

}