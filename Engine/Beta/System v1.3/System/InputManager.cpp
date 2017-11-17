#include "InputManager.h"


System::InputManager::InputManager()
{
	m_MouseCoords = glm::vec2(0.0f, 0.0f);
	m_Camera = nullptr;
}

System::InputManager::~InputManager()
{
}

void System::InputManager::Initialize(std::shared_ptr<Graphics::Camera>& camera)
{
	m_Camera = camera;
}

const glm::vec2 System::InputManager::GetMouseCoords()
{
	return m_MouseCoords;
}

glm::vec3 System::InputManager::GetMouseCoordsWorldSpace()
{
	glm::vec3 worldCoords;
	if (m_Camera)
	{
		glm::vec2 mousePos = GetMouseCoords();
		glm::vec2 normalized = NormalizeDeviceMouseCoords(mousePos);
		glm::vec4 clipCoords = glm::vec4(normalized, -1.0f, 1.0f);
		glm::vec4 eyeCoords = GetEyeSpaceCoords(clipCoords);
		worldCoords = GetWorldSpaceCoords(eyeCoords);
	}
	return worldCoords;
}

glm::vec3 System::InputManager::GetMouse3DPosition()
{
	glm::vec3 origin = m_Camera->GetPosition();
	glm::vec3 mousePosition = GetMouseCoordsWorldSpace();
	glm::vec3 planeNormal = glm::vec3(0, 1, 0);
	float farDistance = m_Camera->GetFarPlane() / 2;
	float DotPt1 = glm::dot(origin, planeNormal);
	float DotPt2 = glm::dot(mousePosition, planeNormal);
	float distanceToPlane = 0.0f - (DotPt1 / DotPt2);
	glm::vec3 mouse3Dposition = origin + (mousePosition * distanceToPlane);
	mousePosition.y = 0.0f;
	return mouse3Dposition;
}

glm::vec3 System::InputManager::GetMouse3DPosition2()
{
	glm::vec3 cameraPos = m_Camera->GetPosition();
	glm::vec3 mousePos = GetMouseCoordsWorldSpace();
	float t = -cameraPos.y / mousePos.y;
	glm::vec3 mousePosition = cameraPos + (mousePos * t);
	mousePosition.y = 0.0f;
	return mousePosition;

}

void System::InputManager::SetMouseCoords(uint16_t x, uint16_t y)
{
	m_MouseCoords.x = (float)x;
	m_MouseCoords.y = (float)y;
}

glm::vec2 System::InputManager::NormalizeDeviceMouseCoords(glm::vec2 & mousePos)
{
	glm::vec2 mousePosition;
	if (m_Camera)
	{
		mousePosition.x = (2.0f * mousePos.x) / m_Camera->GetWindowWidth() - 1.0f;
		mousePosition.y = 1.0f - (2.0f * mousePos.y) / m_Camera->GetWindowHeight();
	}
	return mousePosition;
}

glm::vec4 System::InputManager::GetEyeSpaceCoords(glm::vec4 mouseCoords)
{
	glm::vec4 EyeSpaceCoords;
	if (m_Camera)
	{
		EyeSpaceCoords = glm::inverse(m_Camera->Get3DProjectionMatrix()) * mouseCoords;
		EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
	}
	return EyeSpaceCoords;
}

glm::vec3 System::InputManager::GetWorldSpaceCoords(glm::vec4 eyeCoords)
{
	glm::vec3 worldCoords;
	if (m_Camera)
	{
		glm::vec4 mouseCoords = glm::inverse(m_Camera->GetViewMatrix()) * eyeCoords;
		worldCoords = glm::vec3(mouseCoords.x, mouseCoords.y, mouseCoords.z);
		worldCoords = glm::normalize(worldCoords);
	}
	return worldCoords;
}

void System::InputManager::SetKeyUp(const int keyID)
{
	if (m_KeysDown[(Key)keyID] == true)
		m_KeysPressed[(Key)keyID] = true;

	m_KeysDown[(Key)keyID] = false;
}

void System::InputManager::SetKeyDown(const int iKeyCode)
{
	m_KeysDown[(Key)iKeyCode] = true;
}

bool System::InputManager::KeyStatus(const Key iKeyCode)
{
	auto iterator = m_KeysDown.find((int)iKeyCode);
	if (iterator != m_KeysDown.end())
	{
		return iterator->second;
	}
	return false;
}

bool System::InputManager::KeyPressed(const Key iKeyCode)
{
	bool bPressed = false;
	auto result = m_KeysPressed.find((int)iKeyCode);
	if (result != m_KeysPressed.end())
	{
		bPressed = result->second;
		result->second = false;
	}
	return bPressed;
}

void System::InputManager::Reset()
{
	m_KeysDown.clear();
	m_KeysPressed.clear();
}