#pragma once
#include "winfunc.h"

#ifdef SYSTEM_EXPORTS
#define AppDLL_API __declspec(dllexport)
#else
#define AppDLL_API __declspec(dllimport)
#endif


namespace System
{
	class App
	{
	public:
		AppDLL_API App();
		AppDLL_API void CloseApp();
		AppDLL_API void GetWindowSize(uint32_t& width, uint32_t& height);
		AppDLL_API void SetWindowSize(uint32_t width, uint32_t height);
		AppDLL_API void* GetHandle();
		AppDLL_API void SetHandle(void* handle);

		//state changes
		virtual void Tick() = 0;
		virtual void OnInitialize() = 0;
		virtual void OnActivated() = 0;
		virtual void OnDeactivated() = 0;
		virtual void OnSuspending() = 0;
		virtual void OnResuming() = 0;
		virtual void OnWindowMoved() = 0;
		virtual void OnWindowSizeChanged(int width, int height) = 0;

	private:
		void* m_handle;
		uint32_t m_width;
		uint32_t m_height;
	};

}