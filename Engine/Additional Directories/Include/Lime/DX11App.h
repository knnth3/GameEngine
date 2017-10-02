#pragma once
#include <Windows.h>
#include "Primitives.h"
#include "DX11Graphics.h"
#include "InputManager.h"

namespace Lime
{
	//Forward Decl
	class WinProc;

	class DX11App
	{
	public:
		DLL_EXPORT DX11App();
		DLL_EXPORT ~DX11App();
		//Return 0 to close program.
		DLL_EXPORT virtual void Tick() = 0;
		//Return 0 upon success.
		DLL_EXPORT virtual void Init(HWND wnd) final;
		DLL_EXPORT virtual void CloseApp() final;
		DLL_EXPORT virtual void GetDefaultSize(UINT& width, UINT& height);
		DLL_EXPORT virtual void SetSize(UINT width, UINT height) final;

		//state changes
		DLL_EXPORT virtual void OnActivated() = 0;
		DLL_EXPORT virtual void OnDeactivated() = 0;
		DLL_EXPORT virtual void OnSuspending() = 0;
		DLL_EXPORT virtual void OnResuming() = 0;
		DLL_EXPORT virtual void OnWindowMoved() = 0;
		DLL_EXPORT virtual void OnWindowSizeChanged(int width, int height) = 0;

	protected:
		friend class WinProc;
		void KeyUp(unsigned int Key);
		void KeyDown(unsigned int Key);
		void SetMouseCoords(short x, short y);
		virtual void Initialize() = 0;
		void SetWindowSize(UINT width, UINT height);

		std::unique_ptr<DX11Graphics> m_graphicsDevice;
		std::unique_ptr<InputManager> m_input;
	private:

		UINT m_width;
		UINT m_height;

	};

}