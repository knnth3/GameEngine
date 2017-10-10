#include "Lime.h"

int Lime::AppStarter::Start(std::wstring appName, std::unique_ptr<App>& app)
{
	int result = 0;

	auto m_window = std::make_unique<Win32Window>(appName.c_str(), app);
	if (m_window->Initialize() == 0)
		result = m_window->Run();

	return result;
}
