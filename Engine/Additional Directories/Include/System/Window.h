#pragma once
#include "Window_Win32.h"
#include "App.h"
#include <memory>

namespace System
{
	AppDLL_API int RunApp(std::wstring appName, std::unique_ptr<App>& app);
}