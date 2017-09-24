#pragma once
#include <unordered_map>
#include <glm\glm.hpp>
#include <SDL2\SDL.h>
#include "Camera.h"
#include <glm\gtx/transform.hpp>

namespace Lime
{

    class InputManager
    {
    public:
        DLL_EXPORT InputManager();
        DLL_EXPORT bool Update();
        DLL_EXPORT void LoadCamera(std::shared_ptr<Camera>& camera);
        DLL_EXPORT glm::vec2 GetMouseCoords();
        DLL_EXPORT glm::vec3 GetMouse3DPosition();
        DLL_EXPORT glm::vec3 GetMouse3DPosition2();
        DLL_EXPORT bool KeyStatus(unsigned int keyID);
        DLL_EXPORT bool KeyPressed(unsigned int keyID);
    private:
        void KeyDown(unsigned int keyID);
        void KeyUp(unsigned int keyID);
        void SetMouseCoords(float x, float y);
        glm::vec3 GetMouseCoordsWorldSpace();
        glm::vec2 NormalizeDeviceMouseCoords(glm::vec2& mousePos);
        glm::vec4 GetEyeSpaceCoords(glm::vec4 mouseCoords);
        glm::vec3 GetWorldSpaceCoords(glm::vec4 eyeCoords);

        std::unordered_map<unsigned int, bool> m_KeysDown;
        std::unordered_map<unsigned int, bool> m_KeysPressed;
        std::shared_ptr<Camera> m_Camera;
        glm::vec2 m_MouseCoords;
    };


}