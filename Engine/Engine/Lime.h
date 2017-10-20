#pragma once
#include "App.h"

namespace Lime
{

	// Indicates to hybrid graphics systems to prefer the discrete part by default
	extern "C"
	{
		__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	}

	//Use the Lime::App template to create class for Start
	class AppStarter
	{
	public:
		AppDLL_API static int Start(std::wstring appName, std::unique_ptr<App>& app);
	};
}
