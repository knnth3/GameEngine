#include "pch.h"
#include "TestGame_UWPMain.h"
#include "Common\DirectXHelper.h"

using namespace TestGame_UWP;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Graphics;

// Loads and initializes application assets when the application is loaded.
TestGame_UWPMain::TestGame_UWPMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
	m_model.Scale(100.0f, 100.0f, 100.0f);
	m_deviceResources->GetCamera()->AttachToModel(m_model);
	CreateWindowSizeDependentResources();
}

TestGame_UWPMain::~TestGame_UWPMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void TestGame_UWPMain::CreateWindowSizeDependentResources() 
{
	auto dims = m_deviceResources->GetLogicalSize();
	m_str.SetPosition(100, 100);
	// TODO: Replace this with the size-dependent initialization of your app's content.
}

// Updates the application state once per frame.
void TestGame_UWPMain::Update() 
{
	static float rotation = 0.0f;
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		uint32 fps = m_timer.GetFramesPerSecond();
		m_str = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";
		rotation += (float)m_timer.GetElapsedSeconds();
		m_model.Rotate(rotation, 0.0f, 0.0f);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool TestGame_UWPMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	m_deviceResources->BeginScene(0.2f, 0.2f, 0.2f);
	m_deviceResources->Draw(m_model);
	m_deviceResources->Draw(m_str);
	m_deviceResources->EndScene();

	return true;
}

// Notifies renderers that device resources need to be released.
void TestGame_UWPMain::OnDeviceLost()
{
	m_deviceResources->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void TestGame_UWPMain::OnDeviceRestored()
{
	m_deviceResources->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
