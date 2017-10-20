#pragma once
#include "Shared.h"
#include "InputManager.h"
#include "ModelLoader.h"


#if PLATFORM == OS_WINDOWS
#include "DX11Graphics.h"
#endif

namespace Lime
{
	enum class GRAPHICS_API
	{
		NONE,
		DIRECTX_11,
		OPENGL_4_0
	};

	class App
	{
		friend class Win32Window;
		friend class WinProc;
	public:
		AppDLL_API App(const GRAPHICS_API api);
		AppDLL_API ~App();

	protected:
		//Function for window to run on success.
		void Init(void* hwnd);

		AppDLL_API virtual void Tick() = 0;
		AppDLL_API virtual void CloseApp() final;
		AppDLL_API virtual void GetWindowSize(uint32_t& width, uint32_t& height);
		AppDLL_API virtual void SetSize(uint32_t width, uint32_t height) final;

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
		void SetWindowSize(uint32_t width, uint32_t height);

		//Functions to manipulate Graphics API
		std::shared_ptr<RenderBatch> m_renderBatch;
		std::shared_ptr<InputManager> m_input;

	private:
		GRAPHICS_API m_type;
		uint32_t m_width;
		uint32_t m_height;
	};

}