#pragma once
#include "DllSettings.h"
#include <SDL2\SDL.h>
#include <unordered_map>

namespace System
{
	typedef uint8_t Uint8;

    class SDL_INPUT
    {
    public:
		SDL_INPUT();
        bool KeyStatus(const int iKeyCode);
		void SetStatus(const int iKeyCode, bool value);

		void GetMousePos(uint16_t& x, uint16_t& y);
		void SetMousePos(uint16_t x, uint16_t y);
    private:
		uint16_t m_xMousePos;
		uint16_t m_yMousePos;
		std::unordered_map<int, bool> m_KeysDown;
    };


}