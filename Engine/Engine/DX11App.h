#pragma once
#include <Windows.h>
#include "Lime.h"
#include "DX11Graphics.h"
#include "InputManager.h"

namespace Lime
{
	//Forward Decl
	class WinProc;

	class DX11App
	{
	public:
		AppDLL_API DX11App();
		AppDLL_API ~DX11App();
		//Return 0 to close program.
		AppDLL_API virtual void Tick() = 0;
		//Return 0 upon success.
		AppDLL_API virtual void Init(HWND wnd) final;
		AppDLL_API virtual void CloseApp() final;
		AppDLL_API virtual void GetDefaultSize(UINT& width, UINT& height);
		AppDLL_API virtual void SetSize(UINT width, UINT height) final;

		//state changes
		AppDLL_API virtual void OnActivated() = 0;
		AppDLL_API virtual void OnDeactivated() = 0;
		AppDLL_API virtual void OnSuspending() = 0;
		AppDLL_API virtual void OnResuming() = 0;
		AppDLL_API virtual void OnWindowMoved() = 0;
		AppDLL_API virtual void OnWindowSizeChanged(int width, int height) = 0;

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