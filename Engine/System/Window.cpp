#include "Window.h"


int System::RunApp(std::wstring appName, std::unique_ptr<App>& app)
{
	int result = 0;
	auto m_window = std::make_unique<Window_Win32>(appName.c_str(), app);
	if (m_window->Initialize())
		result = m_window->Run();

	return result;
}