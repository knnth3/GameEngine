#pragma once
#include "DllSettings.h"
#include <Graphics\Camera.h>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>


namespace System
{
	typedef uint8_t Uint8;
	enum Key
	{
		KEY_UNKNOWN = 0,
		KEY_RETURN = '\r',
		KEY_ESCAPE = '\033',
		KEY_BACKSPACE = '\b',
		KEY_TAB = '\t',
		KEY_SPACE = ' ',
		KEY_EXCLAIM = '!',
		KEY_QUOTEDBL = '"',
		KEY_HASH = '#',
		KEY_PERCENT = '%',
		KEY_DOLLAR = '$',
		KEY_AMPERSAND = '&',
		KEY_QUOTE = '\'',
		KEY_LEFTPAREN = '(',
		KEY_RIGHTPAREN = ')',
		KEY_ASTERISK = '*',
		KEY_PLUS = '+',
		KEY_COMMA = ',',
		KEY_MINUS = '-',
		KEY_PERIOD = '.',
		KEY_SLASH = '/',
		KEY_0 = '0',
		KEY_1 = '1',
		KEY_2 = '2',
		KEY_3 = '3',
		KEY_4 = '4',
		KEY_5 = '5',
		KEY_6 = '6',
		KEY_7 = '7',
		KEY_8 = '8',
		KEY_9 = '9',
		KEY_COLON = ':',
		KEY_SEMICOLON = ';',
		KEY_LESS = '<',
		KEY_EQUALS = '=',
		KEY_GREATER = '>',
		KEY_QUESTION = '?',
		KEY_AT = '@',
		KEY_LEFTBRACKET = '[',
		KEY_BACKSLASH = '\\',
		KEY_RIGHTBRACKET = ']',
		KEY_CARET = '^',
		KEY_UNDERSCORE = '_',
		KEY_BACKQUOTE = '`',
		KEY_a = 'a',
		KEY_b = 'b',
		KEY_c = 'c',
		KEY_d = 'd',
		KEY_e = 'e',
		KEY_f = 'f',
		KEY_g = 'g',
		KEY_h = 'h',
		KEY_i = 'i',
		KEY_j = 'j',
		KEY_k = 'k',
		KEY_l = 'l',
		KEY_m = 'm',
		KEY_n = 'n',
		KEY_o = 'o',
		KEY_p = 'p',
		KEY_q = 'q',
		KEY_r = 'r',
		KEY_s = 's',
		KEY_t = 't',
		KEY_u = 'u',
		KEY_v = 'v',
		KEY_w = 'w',
		KEY_x = 'x',
		KEY_y = 'y',
		KEY_z = 'z',
		KEY_WIN_W = 87,
		KEY_WIN_S = 83,
	};


	class InputManager
	{
	public:
		SYSTEM_DLL_API InputManager();
		SYSTEM_DLL_API ~InputManager();
		SYSTEM_DLL_API void Initialize(std::shared_ptr<Graphics::Camera>& camera);
		SYSTEM_DLL_API const glm::vec2 GetMouseCoords();
		SYSTEM_DLL_API glm::vec3 GetMouse3DPosition();
		SYSTEM_DLL_API glm::vec3 GetMouse3DPosition2();
		SYSTEM_DLL_API bool KeyStatus(const Key iKeyCode);
		SYSTEM_DLL_API bool KeyPressed(const Key iKeyCode);
		SYSTEM_DLL_API void Reset();

	private:
		friend class Window_SDL;
		friend class Window_Win32;
		SYSTEM_DLL_API void SetKeyDown(const int iKeyCode);
		SYSTEM_DLL_API void SetKeyUp(const int iKeyCode);
		SYSTEM_DLL_API void SetMouseCoords(uint16_t x, uint16_t y);

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


}