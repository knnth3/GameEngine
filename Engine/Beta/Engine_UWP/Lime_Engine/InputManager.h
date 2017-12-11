#pragma once
#include "DllSettings.h"
#include <Graphics\Camera.h>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>


namespace LIME_ENGINE
{
	class InputManager
	{
	public:
		ENGINE_DLL_API InputManager();
		ENGINE_DLL_API void Initialize(_Inout_ std::shared_ptr<Graphics::Camera>& camera);
		ENGINE_DLL_API const glm::vec2 GetMouseCoords();
		ENGINE_DLL_API bool GetMouse3DPosition(_Inout_ glm::vec3& pos);
		ENGINE_DLL_API bool GetMouse3DPosition2(_Inout_ glm::vec3& pos);
		ENGINE_DLL_API bool KeyStatus(_In_ int iKeyCode);
		ENGINE_DLL_API bool KeyPressed(_In_ int iKeyCode);
		ENGINE_DLL_API bool LMBPressed();
		ENGINE_DLL_API bool LMBStatus();
		ENGINE_DLL_API bool RMBPressed();
		ENGINE_DLL_API bool RMBStatus();
		ENGINE_DLL_API void Reset();

		//OS usage
		ENGINE_DLL_API void SetKeyDown(_In_ int iKeyCode);
		ENGINE_DLL_API void SetKeyUp(_In_ int iKeyCode);
		ENGINE_DLL_API void SetMouseCoords(_In_ float x, float y);
		ENGINE_DLL_API void SetLMBStatus(bool val);
		ENGINE_DLL_API void SetRMBStatus(bool val);

	private:
		glm::vec3 GetMouseCoordsWorldSpace();
		glm::vec2 NormalizeDeviceMouseCoords(glm::vec2& mousePos);
		glm::vec4 GetEyeSpaceCoords(glm::vec4 mouseCoords);
		glm::vec3 GetWorldSpaceCoords(glm::vec4 eyeCoords);

		std::unordered_map<int, bool> m_KeysDown;
		std::unordered_map<int, bool> m_KeysPressed;
		std::shared_ptr<Graphics::Camera> m_Camera;
		glm::vec2 m_MouseCoords;
		uint16_t m_screenWidth, m_screenHeight;

		//Mouse
		bool m_rmbPressed;
		bool m_lmbPressed;
		bool m_lmbDown;
		bool m_rmbDown;
	};
}