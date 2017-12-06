#pragma once
#include "DllSettings.h"
#include "Console.h"
#include "InputManager.h"
#include "Timer.h"

namespace LIME_ENGINE
{
	class EngineApp
	{
	public:
		ENGINE_DLL_API EngineApp(
			std::shared_ptr<Graphics::GraphicsDevice>& graphicsDevice,
			std::shared_ptr<InputManager>& inputManager,
			std::shared_ptr<StepTimer>& timer);

		ENGINE_DLL_API virtual void Initialize() final;
		ENGINE_DLL_API virtual void Tick() final;
		ENGINE_DLL_API virtual void Render();
		ENGINE_DLL_API virtual void SetClearColor(float r, float g, float b)final;
		ENGINE_DLL_API virtual void SetDimensions(float width, float height)final;

		//Overloadable functions
		ENGINE_DLL_API virtual void OnInitialize();
		ENGINE_DLL_API virtual void OnUpdate();
		ENGINE_DLL_API virtual void OnRender();
		ENGINE_DLL_API virtual void OnActivated();
		ENGINE_DLL_API virtual void OnDeactivated();
		ENGINE_DLL_API virtual void OnSuspending();
		ENGINE_DLL_API virtual void OnResuming();
		ENGINE_DLL_API virtual void OnWindowMoved();
		ENGINE_DLL_API virtual void OnWindowSizeChanged(float width, float height);
		ENGINE_DLL_API virtual void OnShutdown();

	protected:
		//User functions

		//Setters
		ENGINE_DLL_API virtual void SetConsoleOpenButton(int code)final;

		//Getters
		ENGINE_DLL_API virtual std::shared_ptr<Console> GetConsole()final;
		ENGINE_DLL_API virtual std::shared_ptr<Graphics::GraphicsDevice> GetGraphicsDevice()final;
		ENGINE_DLL_API virtual std::shared_ptr<InputManager> GetInputManager()final;
		ENGINE_DLL_API virtual std::shared_ptr<StepTimer> GetTimer()final;

	private:
		void CreateCommonBrushes();

		int m_consoleCode;
		glm::vec3 m_clearColor;
		std::shared_ptr<StepTimer> m_timer;
		std::shared_ptr<Console> m_console;
		std::shared_ptr<Graphics::GraphicsDevice> m_graphicsDevice;
		std::shared_ptr<InputManager> m_inputManager;
	};

}
