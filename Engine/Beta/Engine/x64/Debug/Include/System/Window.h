#pragma once
#include "DllSettings.h"
#include "App.h"

namespace System
{
	class Window
	{
	public:
		virtual bool Initialize() = 0;
		virtual int Run() = 0;
	};

}