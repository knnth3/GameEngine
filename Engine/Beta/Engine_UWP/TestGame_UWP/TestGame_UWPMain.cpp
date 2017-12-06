#include "pch.h"
#include "TestGame_UWPMain.h"
#include "Common\DirectXHelper.h"
#include "TestGame.h"

using namespace TestGame_UWP;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Core;
using namespace Concurrency;
using namespace Graphics;
using namespace LIME_ENGINE;

// Loads and initializes application assets when the application is loaded.
TestGame_UWPMain::TestGame_UWPMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	//Set Default window Dimensions
	ApplicationView::PreferredLaunchViewSize = Size(800, 600);
	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;

	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);
	m_controller = ref new InputController(CoreWindow::GetForCurrentThread());
	m_controller->AttatchCamera(m_deviceResources->GetCamera());
	m_controller->Active(true);
	m_controller->ShowCursor();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
	std::shared_ptr<GraphicsDevice> m_graphics = m_deviceResources;
	m_timer = std::shared_ptr<StepTimer>(new StepTimer());
	m_engineApp = std::make_unique<TestGame>(m_graphics, m_controller->GetManager(), m_timer);
	m_engineApp->Initialize();
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
	float width, height;
	m_deviceResources->GetWindowDimensions(width, height);
	m_engineApp->SetDimensions(width, height);
}

// Updates the application state once per frame.
void TestGame_UWPMain::Update() 
{
	m_controller->Update();
	if (m_controller->GetManager()->KeyPressed((int)Windows::System::VirtualKey::P))
	{
		SwitchFullscreen();
	}

	static float rotation = 0.0f;
	// Update scene objects.
	m_timer->Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		uint32 fps = m_timer->GetFramesPerSecond();
		m_str = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";
		rotation += (float)m_timer->GetElapsedSeconds();
		m_engineApp->Tick();
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool TestGame_UWPMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer->GetFrameCount() == 0)
	{
		return false;
	}
	m_engineApp->Render();
	return true;
}

void TestGame_UWP::TestGame_UWPMain::SwitchFullscreen()
{
	ApplicationView^ view = ApplicationView::GetForCurrentView();
	if (view->IsFullScreenMode)
	{
		view->ExitFullScreenMode();
	}
	else
	{
		view->TryEnterFullScreenMode();
	}
}

// Notifies renderers that device resources need to be released.
void TestGame_UWPMain::OnDeviceLost()
{
	//Close here
	m_deviceResources->ReleaseDeviceDependentResources();
	m_engineApp->OnSuspending();
}

// Notifies renderers that device resources may now be recreated.
void TestGame_UWPMain::OnDeviceRestored()
{
	m_deviceResources->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	m_engineApp->OnResuming();
}
