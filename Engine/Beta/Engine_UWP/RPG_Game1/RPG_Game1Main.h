#pragma once

#include "Common\DeviceResources.h"
#include "Common\UWP_InputControlls.h"
#include <Lime_Engine\EngineApp.h>

// Hnadles Engine app connections to UWP app interface
namespace RPG_Game1
{
	class RPG_Game1Main : public DX::IDeviceNotify
	{
	public:
		RPG_Game1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~RPG_Game1Main();
		void WindowSizeChanged();
		void Update();
		bool Render();
		void Suspend();
		void Resume();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void FullScreen();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<LIME_ENGINE::StepTimer> m_timer;
		std::unique_ptr<LIME_ENGINE::EngineApp> m_engineApp;
		UWP_InputControlls^ m_controller;
	};
}