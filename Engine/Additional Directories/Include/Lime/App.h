#pragma once
#include "Shared.h"
#include "DX11Graphics.h"
#include "InputManager.h"
#include "ModelLoader.h"

namespace Lime
{
	//Forward Decl
	//class WinProc;
	//class Win32Window;


	class App
	{
		friend class Win32Window;
		friend class WinProc;
	public:
		AppDLL_API App();
		AppDLL_API ~App();

	protected:
		//Function for window to run on success.
		void Init(HWND hwnd);

		AppDLL_API virtual void Tick() = 0;
		AppDLL_API virtual void CloseApp() final;
		AppDLL_API virtual void GetWindowSize(UINT& width, UINT& height);
		AppDLL_API virtual void SetSize(UINT width, UINT height) final;

		//state changes
		AppDLL_API virtual void OnInitialize() = 0;
		AppDLL_API virtual void OnActivated() = 0;
		AppDLL_API virtual void OnDeactivated() = 0;
		AppDLL_API virtual void OnSuspending() = 0;
		AppDLL_API virtual void OnResuming() = 0;
		AppDLL_API virtual void OnWindowMoved() = 0;
		AppDLL_API virtual void OnWindowSizeChanged(int width, int height) = 0;

		//Functions for the OS
		void KeyUp(unsigned int Key);
		void KeyDown(unsigned int Key);
		void SetMouseCoords(short x, short y);
		void SetWindowSize(UINT width, UINT height);

		//Functions to manipulate Graphics API
		std::shared_ptr<DX11Graphics> m_graphicsDevice;
		std::shared_ptr<InputManager> m_input;

	private:

		HWND m_hwnd;
		UINT m_width;
		UINT m_height;
	};

}