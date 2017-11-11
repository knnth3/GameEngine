#pragma once
#include "DllSettings.h"
#include "App.h"

namespace System
{
	enum flags
	{
		NONE = 0x0,
		WIN32_WINDOW = 0x1,
		DIRECTX = 0x2,
		VSYNC = 0x4,
		FULLSCREEN = 0x8,
	};

	//Flags to pass can be found under System::flags namespace
	SYSTEM_DLL_API int RunApp(std::string& appName, std::unique_ptr<App>& app, uint32_t flags = NONE);
}