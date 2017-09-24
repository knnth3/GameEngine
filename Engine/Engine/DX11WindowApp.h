#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Primitives.h"

#define WIN32_LEAN_AND_MEAN
#define FULL_SCREEN false

namespace Lime
{
	DLL_EXPORT LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	class DX11WindowApp
	{
	public:
		DLL_EXPORT DX11WindowApp(LPCWSTR AppName);
		DLL_EXPORT ~DX11WindowApp();
		//Returns false on close
		DLL_EXPORT bool Run();
		DLL_EXPORT const HWND GetHWND();
		DLL_EXPORT unsigned int Width()const;
		DLL_EXPORT unsigned int Height()const;
	private:
		unsigned int m_width;
		unsigned int m_height;
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
	};

}