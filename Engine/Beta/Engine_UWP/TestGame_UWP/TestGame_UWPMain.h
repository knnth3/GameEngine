#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include <Graphics\RenderBatch_2D.h>


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

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		//Render Objects
		Graphics::Model m_model;
		Graphics::Text m_str;
	};
}