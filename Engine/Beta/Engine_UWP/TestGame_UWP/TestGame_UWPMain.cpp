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

	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);
	m_controller = ref new InputController(CoreWindow::GetForCurrentThread());
	m_controller->AttatchCamera(m_deviceResources->GetCamera());
	m_controller->Active(true);
	m_controller->ShowCursor();

	std::shared_ptr<GraphicsDevice> m_graphics = m_deviceResources;
	m_timer = std::shared_ptr<StepTimer>(new StepTimer());
	m_engineApp = std::make_unique<TestGame>(m_graphics, m_controller->GetManager(), m_timer);
	m_engineApp->RegisterGraphicsDevice(m_graphics);
	m_engineApp->RegisterInputManager(m_controller->GetManager());
	m_engineApp->RegisterTimer(m_timer);
	m_engineApp->Resume();

	//Set Default window Dimensions
	float defaultWidth, defaultHeight;
	m_engineApp->GetDefaultDimensions(defaultWidth, defaultHeight);
	ApplicationView::PreferredLaunchViewSize = Size(defaultWidth, defaultHeight);
	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;
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
	m_engineApp->UpdateDimensions();
}

// Updates the application state once per frame.
void TestGame_UWPMain::Update() 
{
	m_controller->Update();
	if (m_controller->GetManager()->KeyPressed((int)Windows::System::VirtualKey::P))
	{
		SwitchFullscreen();
	}
	// Update scene objects.
	m_timer->Tick([&]()
	{
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
}

// Notifies renderers that device resources may now be recreated.
void TestGame_UWPMain::OnDeviceRestored()
{
	m_deviceResources->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
