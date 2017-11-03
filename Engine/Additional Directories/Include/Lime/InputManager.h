#pragma once
#include <unordered_map>
#include <glm\glm.hpp>
#include "Camera.h"
#include <glm\gtx\transform.hpp>

#if PLATFORM == OS_WINDOWS
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
#endif


namespace Lime
{
	enum Key
	{
		Left_Mouse_Button = 0x01,
		A = 0x41,
		D = 0x44,
		W = 0x57,
		S = 0x53,
		Q = 0x51,
		E = 0x45,
		P = 0x50,
		ESC = 0x1B,
	};


	//Forward Decl
	class App;

    class InputManager
    {
    public:
        AppDLL_API InputManager();
		AppDLL_API ~InputManager();
		AppDLL_API void Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
        AppDLL_API void LoadCamera(std::shared_ptr<Camera>& camera);
        AppDLL_API glm::vec2 GetMouseCoords();
        AppDLL_API glm::vec3 GetMouse3DPosition();
        AppDLL_API glm::vec3 GetMouse3DPosition2();
        AppDLL_API bool KeyStatus(Key keyID);
        AppDLL_API bool KeyPressed(Key keyID);
		AppDLL_API void Reset();
	protected:
		friend class App;
		void KeyDown(unsigned int keyID);
		void KeyUp(unsigned int keyID);
		void SetMouseCoords(short x, short y);
    private:
        glm::vec3 GetMouseCoordsWorldSpace();
        glm::vec2 NormalizeDeviceMouseCoords(glm::vec2& mousePos);
        glm::vec4 GetEyeSpaceCoords(glm::vec4 mouseCoords);
        glm::vec3 GetWorldSpaceCoords(glm::vec4 eyeCoords);

        std::unordered_map<Key, bool> m_KeysDown;
        std::unordered_map<Key, bool> m_KeysPressed;
        std::shared_ptr<Camera> m_Camera;
        glm::vec2 m_MouseCoords;
		uint16_t m_screenWidth, m_screenHeight;

    };


}