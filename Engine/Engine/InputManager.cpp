#include "InputManager.h"
#include <iostream>

namespace Lime
{

    InputManager::InputManager()
    {
		m_directInput = NULL;
		m_keyboard = NULL;
		m_mouse = NULL;
        m_MouseCoords = glm::vec2(0.0f, 0.0f);
        m_Camera = nullptr;
    }
	InputManager::~InputManager()
	{
	}
	void InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

#if PLATFORM == OS_WINDOWS
		//Windows is set to pass input through
		//KeyUp(key) & KeyDown(key)
		//No need to init as it is a default function
#endif
	}

    void InputManager::LoadCamera(std::shared_ptr<Camera>& camera)
    {
        m_Camera = camera;
    }

    glm::vec2 InputManager::GetMouseCoords()
    {
        return m_MouseCoords;
    }

    glm::vec3 InputManager::GetMouseCoordsWorldSpace()
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

    glm::vec3 InputManager::GetMouse3DPosition()
    {
        glm::vec3 origin = m_Camera->GetPosition();
        glm::vec3 mousePosition = GetMouseCoordsWorldSpace();
        glm::vec3 planeNormal = glm::vec3(0, 1, 0);
        float farDistance = m_Camera->GetFarPlane() / 2;
        float DotPt1 = glm::dot(origin, planeNormal);
        float DotPt2 = glm::dot(mousePosition, planeNormal);
        float distanceToPlane = 0.0f - (DotPt1 / DotPt2);
        glm::vec3 mouse3Dposition = origin + (mousePosition * distanceToPlane);
        return mouse3Dposition;
    }

    glm::vec3 InputManager::GetMouse3DPosition2()
    {
        glm::vec3 cameraPos = m_Camera->GetPosition();
        glm::vec3 mousePos = GetMouseCoordsWorldSpace();
        float t = -cameraPos.y / mousePos.y;
        glm::vec3 mousePosition = cameraPos + (mousePos * t);
        return mousePosition;

    }

    void InputManager::SetMouseCoords(short x, short y)
    {
        m_MouseCoords.x = (float)x;
        m_MouseCoords.y = (float)y;
    }

    glm::vec2 InputManager::NormalizeDeviceMouseCoords(glm::vec2 & mousePos)
    {
        glm::vec2 mousePosition;
        if (m_Camera)
        {
            mousePosition.x = (2.0f * mousePos.x) / m_Camera->GetWindowWidth() - 1.0f;
            mousePosition.y = 1.0f - (2.0f * mousePos.y) / m_Camera->GetWindowHeight();
        }
        return mousePosition;
    }

    glm::vec4 InputManager::GetEyeSpaceCoords(glm::vec4 mouseCoords)
    {
        glm::vec4 EyeSpaceCoords;
        if (m_Camera)
        {
            EyeSpaceCoords = glm::inverse(m_Camera->Get3DProjectionMatrix()) * mouseCoords;
            EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
        }
        return EyeSpaceCoords;
    }

    glm::vec3 InputManager::GetWorldSpaceCoords(glm::vec4 eyeCoords)
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

    void InputManager::KeyUp(unsigned int keyID)
    {
        if (m_KeysDown[(Key)keyID] == true)
            m_KeysPressed[(Key)keyID] = true;

        m_KeysDown[(Key)keyID] = false;
    }

	void InputManager::KeyDown(unsigned int keyID)
    {
        m_KeysDown[(Key)keyID] = true;
    }

    bool InputManager::KeyStatus(Key keyID)
    {
        auto iterator = m_KeysDown.find(keyID);
        if (iterator != m_KeysDown.end())
        {
            return iterator->second;
        }
        return false;
    }
    AppDLL_API bool InputManager::KeyPressed(Key keyID)
    {
        bool bWasPressed = m_KeysPressed[keyID];
        if(bWasPressed)
            m_KeysPressed[keyID] = false;

        return bWasPressed;
    }
}