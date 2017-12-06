#pragma once
#include "DllSettings.h"
#include <Graphics\Camera.h>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

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
	ENGINE_DLL_API void Reset();

public:
	ENGINE_DLL_API void SetKeyDown(_In_ int iKeyCode);
	ENGINE_DLL_API void SetKeyUp(_In_ int iKeyCode);
	ENGINE_DLL_API void SetMouseCoords(_In_ float x, float y);

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

};