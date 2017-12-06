#pragma once

#include "Common\DeviceResources.h"
#include "Common\InputController.h"
#include <Lime_Engine\EngineApp.h>

// Renders Direct2D and 3D content on the screen.

namespace TestGame_UWP
{
	class TestGame_UWPMain : public DX::IDeviceNotify
	{
	public:
		TestGame_UWPMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~TestGame_UWPMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();
		void SwitchFullscreen();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();


	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Rendering loop timer.
		std::shared_ptr<LIME_ENGINE::StepTimer> m_timer;

		//Render Objects
		Graphics::Text m_str;
		std::unique_ptr<LIME_ENGINE::EngineApp> m_engineApp;
		InputController^ m_controller;
	};
}