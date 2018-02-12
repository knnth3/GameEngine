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
		ENGINE_DLL_API EngineApp();
		ENGINE_DLL_API void RegisterGraphicsDevice(std::shared_ptr<Graphics::GraphicsDevice> graphicsDevice);
		ENGINE_DLL_API void RegisterInputManager(std::shared_ptr<InputManager> inputManager);
		ENGINE_DLL_API void RegisterTimer(std::shared_ptr<StepTimer> timer);
		ENGINE_DLL_API void GetDefaultDimensions(float& width, float& height);
		ENGINE_DLL_API void Tick();
		ENGINE_DLL_API void Render();
		ENGINE_DLL_API void UpdateDimensions();
		ENGINE_DLL_API void Suspend();
		ENGINE_DLL_API void Resume();

	protected:

		//Required
		ENGINE_DLL_API virtual void OnUpdate() = 0;
		ENGINE_DLL_API virtual void OnRender() = 0;
		ENGINE_DLL_API virtual void OnResume() = 0;
		ENGINE_DLL_API virtual void OnSuspend() = 0;

		//Optional
		ENGINE_DLL_API virtual void OnWindowVisibilityChange();
		ENGINE_DLL_API virtual void OnWindowSizeChanged();
		ENGINE_DLL_API virtual void OnWindowMoved();

		//Usage
		ENGINE_DLL_API void SetDefaultDimensions(float width, float height);

	private:
		bool m_bActivated;
		glm::vec2 m_defaultDimensions;
		glm::vec3 m_clearColor;
	};

}
