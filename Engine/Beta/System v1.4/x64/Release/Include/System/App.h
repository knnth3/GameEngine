#pragma once
#include "DllSettings.h"
#include "InputManager.h"
#include "AppSettings.h"
#include <Graphics\Graphics.h>

namespace System
{

	class App
	{
	public:
		SYSTEM_DLL_API App(uint16_t windowWidth, uint16_t windowHeight, std::string programPath);
		SYSTEM_DLL_API ~App();
		SYSTEM_DLL_API virtual void CloseApp(int code = 0) final;
		SYSTEM_DLL_API virtual void GetWindowSize(uint16_t& width, uint16_t& height) final;
		SYSTEM_DLL_API virtual void SetWindowSize(uint16_t  width, uint16_t  height) final;
		SYSTEM_DLL_API virtual bool IsClosing() final;
		SYSTEM_DLL_API virtual void Initialize(std::shared_ptr<Graphics::GraphicsDevice>& graphics) final;
		SYSTEM_DLL_API virtual std::shared_ptr<Graphics::Camera> GetCamera() final;
		SYSTEM_DLL_API virtual std::shared_ptr<Graphics::GraphicsDevice> GetGraphicsDevice() final;
		SYSTEM_DLL_API virtual std::shared_ptr<System::InputManager> GetInputManager() final;
		SYSTEM_DLL_API virtual int GetOnExitCode() final;

		//state changes (Use these to run code on certain events)

		//Overridable
		SYSTEM_DLL_API virtual void Tick();
		//Overridable
		SYSTEM_DLL_API virtual void OnInitialize();
		//Overridable
		SYSTEM_DLL_API virtual void OnActivated();
		//Overridable
		SYSTEM_DLL_API virtual void OnDeactivated();
		//Overridable
		SYSTEM_DLL_API virtual void OnSuspending();
		//Overridable
		SYSTEM_DLL_API virtual void OnResuming();
		//Overridable
		SYSTEM_DLL_API virtual void OnWindowMoved();
		//Overridable
		SYSTEM_DLL_API virtual void OnWindowSizeChanged(int width, int height);

	private:

		bool m_bIsClosing;
		int m_onExitCode;
		uint16_t m_width;
		uint16_t m_height;
		AppSettings m_settings;
		std::shared_ptr<Graphics::Camera> m_camera;
		std::shared_ptr<Graphics::GraphicsDevice> m_graphics;
		std::shared_ptr<System::InputManager> m_inputManager;
	};

}