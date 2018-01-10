#pragma once
#include "DllSettings.h"
#include "Console.h"
#include <System\System.h>

namespace LIME_ENGINE
{
	class EngineApp:
		public System::App
	{
	public:
		ENGINE_DLL_API EngineApp(uint16_t windowWidth, uint16_t windowHeight, std::string programPath);
		ENGINE_DLL_API ~EngineApp();

		ENGINE_DLL_API virtual void OnInitialize() final;
		ENGINE_DLL_API virtual void Tick() final;

		//Overloadable functions
		ENGINE_DLL_API virtual void OnTick();
		ENGINE_DLL_API virtual void OnStartUp();
		ENGINE_DLL_API virtual void OnActivated();
		ENGINE_DLL_API virtual void OnDeactivated();
		ENGINE_DLL_API virtual void OnSuspending();
		ENGINE_DLL_API virtual void OnResuming();
		ENGINE_DLL_API virtual void OnWindowMoved();
		ENGINE_DLL_API virtual void OnWindowSizeChanged(int width, int height);
		ENGINE_DLL_API virtual void OnShutdown();

		//User functions
		ENGINE_DLL_API std::shared_ptr<Console> GetConsole();

	private:

		uint16_t m_windowWidth;
		uint16_t m_windowHeight;
		std::shared_ptr<Console> m_console;
	};

}
