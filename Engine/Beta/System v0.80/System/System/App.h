#pragma once
#include "DllSettings.h"
#include <Graphics\Graphics.h>

namespace System
{

	class App
	{
	public:
		AppDLL_API App(uint16_t windowWidth, uint16_t windowHeight);
		AppDLL_API ~App();
		AppDLL_API virtual void CloseApp(int code = 0) final;
		AppDLL_API virtual void GetWindowSize(uint16_t& width, uint16_t& height) final;
		AppDLL_API virtual void SetWindowSize(uint16_t  width, uint16_t  height) final;
		AppDLL_API virtual void GetDefaultWindowSize(uint16_t& width, uint16_t& height) final;
		AppDLL_API virtual void SetDefaultWindowSize(uint16_t  width, uint16_t  height) final;
		AppDLL_API virtual bool IsClosing() final;
		AppDLL_API virtual void Initialize(std::shared_ptr<Graphics::Camera>& camera, 
			std::shared_ptr<Graphics::GraphicsDevice>& graphics);
		AppDLL_API virtual std::shared_ptr<Graphics::Camera> GetCamera() final;
		AppDLL_API virtual std::shared_ptr<Graphics::GraphicsDevice> GetGraphicsDevice() final;
		AppDLL_API virtual int GetOnExitCode() final;

		//state changes (Use these to run code on certain events)

		//Overridable
		AppDLL_API virtual void Tick();
		//Overridable
		AppDLL_API virtual void OnInitialize();
		//Overridable
		AppDLL_API virtual void OnActivated();
		//Overridable
		AppDLL_API virtual void OnDeactivated();
		//Overridable
		AppDLL_API virtual void OnSuspending();
		//Overridable
		AppDLL_API virtual void OnResuming();
		//Overridable
		AppDLL_API virtual void OnWindowMoved();
		//Overridable
		AppDLL_API virtual void OnWindowSizeChanged(int width, int height);

	private:
		bool m_bIsClosing;
		int m_onExitCode;
		uint16_t m_width;
		uint16_t m_height;
		uint16_t m_defaultWidth;
		uint16_t m_defaultHeight;
		std::shared_ptr<Graphics::Camera> m_camera;
		std::shared_ptr<Graphics::GraphicsDevice> m_graphics;
	};

}