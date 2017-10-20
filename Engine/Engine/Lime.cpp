#include "Lime.h"

#if PLATFORM == OS_WINDOWS
#include "Win32Window.h"
#endif

int Lime::AppStarter::Start(std::wstring appName, std::unique_ptr<App>& app)
{
	int result = 0;

#if PLATFORM == OS_WINDOWS

	auto m_window = std::make_unique<Win32Window>(appName.c_str(), app);
	if (m_window->Initialize() == 0)
		result = m_window->Run();

#endif

	return result;
}
