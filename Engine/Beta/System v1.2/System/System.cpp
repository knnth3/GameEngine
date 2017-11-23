#include "System.h"
#include "Window_Win32.h"
#include "Window_SDL.h"


int System::RunApp(std::string& appName, std::unique_ptr<App>& app, uint32_t flags)
{
	int result = 0;
	std::unique_ptr<Window> m_application = nullptr;

	if (flags & WIN32_WINDOW)
		m_application = std::make_unique<Window_Win32>(appName, app, false);
	else
		m_application = std::make_unique<Window_SDL>(appName, app, false);

	if(m_application)
		if (m_application->Initialize())
			result = m_application->Run();

	return result;
}