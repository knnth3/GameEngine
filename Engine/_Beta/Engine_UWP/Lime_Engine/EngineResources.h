#pragma once
#include <Graphics\Graphics.h>
#include "InputManager.h"
#include "Timer.h"
#include "Console.h"


namespace LIME_ENGINE
{
	class EngineResources
	{
	public:
		ENGINE_DLL_API static bool Initialize();

		ENGINE_DLL_API static void RegisterGraphicsDevice(std::shared_ptr<Graphics::GraphicsDevice> graphicsDevice);
		ENGINE_DLL_API static void RegisterInputManager(std::shared_ptr<InputManager> inputManager);
		ENGINE_DLL_API static void RegisterTimer(std::shared_ptr<StepTimer> timer);
		ENGINE_DLL_API static void Reset();
		ENGINE_DLL_API static std::shared_ptr<Graphics::GraphicsDevice> GetGraphicsDevice();
		ENGINE_DLL_API static std::shared_ptr<InputManager> GetInputManager();
		ENGINE_DLL_API static std::shared_ptr<StepTimer> GetTimer();
		ENGINE_DLL_API static std::shared_ptr<Console> GetConsole();

	private:

		static std::shared_ptr<Graphics::GraphicsDevice> m_graphicsDevice;
		static std::shared_ptr<InputManager> m_inputManager;
		static std::shared_ptr<StepTimer> m_timer;
		static std::shared_ptr<Console> m_console;
	};

}
