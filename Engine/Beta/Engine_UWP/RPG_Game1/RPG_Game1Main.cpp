#include "pch.h"
#include "RPG_Game1Main.h"
#include "Common\DirectXHelper.h"
#include "RPG_Game_App.h"

using namespace Graphics;
using namespace LIME_ENGINE;
using namespace RPG_Game1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Core;
using namespace Concurrency;


// Loads and initializes application assets when the application is loaded.
RPG_Game1Main::RPG_Game1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources):
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);
	m_engineApp = std::make_unique<RPG_Game_App>();
	m_timer = std::shared_ptr<StepTimer>(new StepTimer());
	m_controller = ref new UWP_InputControlls(CoreWindow::GetForCurrentThread());
	m_controller->AttatchCamera(m_deviceResources->GetCamera());
	m_controller->Active(true);
	m_controller->ShowCursor();

	//Set Default window Dimensions
	float defaultWidth, defaultHeight;
	m_engineApp->GetDefaultDimensions(defaultWidth, defaultHeight);
	ApplicationView::PreferredLaunchViewSize = Size(defaultWidth, defaultHeight);
	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;
}

RPG_Game1Main::~RPG_Game1Main()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void RPG_Game1Main::WindowSizeChanged() 
{
	m_engineApp->UpdateDimensions();
}

// Updates the application state once per frame.
void RPG_Game1Main::Update() 
{
	m_controller->Update();
	if (m_controller->GetManager()->KeyPressed((int)Windows::System::VirtualKey::P))
	{
		FullScreen();
	}
	// Update scene objects.
	m_timer->Tick([&]()
	{
		m_engineApp->Tick();
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool RPG_Game1Main::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer->GetFrameCount() == 0)
	{
		return false;
	}
	m_engineApp->Render();
	return true;
}

void RPG_Game1::RPG_Game1Main::Suspend()
{
	m_engineApp->Suspend();
}

void RPG_Game1::RPG_Game1Main::Resume()
{
	m_engineApp->RegisterGraphicsDevice(m_deviceResources);
	m_engineApp->RegisterInputManager(m_controller->GetManager());
	m_engineApp->RegisterTimer(m_timer);
	m_engineApp->Resume();
}

// Notifies renderers that device resources need to be released.
void RPG_Game1Main::OnDeviceLost()
{
	//ShutDownScene here
	m_deviceResources->ReleaseDeviceDependentResources();
	m_engineApp->Suspend();
}

// Notifies renderers that device resources may now be recreated.
void RPG_Game1Main::OnDeviceRestored()
{
	m_deviceResources->CreateDeviceDependentResources();
	m_engineApp->Resume();
}

void RPG_Game1::RPG_Game1Main::FullScreen()
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