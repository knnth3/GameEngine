#pragma once
#include <Windows.h>
#include "Primitives.h"
#include "DX11Graphics.h"

namespace Lime
{
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

		DLL_EXPORT virtual void SetSize(UINT width, UINT height) final;
		DLL_EXPORT virtual void GetDefaultSize(int& width, int& height) const final;

		//state changes
		DLL_EXPORT virtual void OnActivated() = 0;
		DLL_EXPORT virtual void OnDeactivated() = 0;
		DLL_EXPORT virtual void OnSuspending() = 0;
		DLL_EXPORT virtual void OnResuming() = 0;
		DLL_EXPORT virtual void OnWindowMoved() = 0;
		DLL_EXPORT virtual void OnWindowSizeChanged(int width, int height) = 0;

	protected:
		virtual void Initialize() = 0;
		void SetWindowSize(UINT width, UINT height);

		std::unique_ptr<DX11Graphics> m_graphicsDevice;
	private:
		UINT m_width;
		UINT m_height;

	};
}

