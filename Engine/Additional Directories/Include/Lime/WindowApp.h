#pragma once

#include <string>
#include <SDL2\SDL.h>
#include <gl\glew.h>
#include "Primitives.h"

namespace Lime
{

    class WindowApp
    {
    public:
        DLL_EXPORT WindowApp();
        //Returns true if started correctly if else, returns an error code number for SDL
        DLL_EXPORT int Initialize(std::string windowName, int windownWidth, int windowHeight, unsigned int flags = 0);
        DLL_EXPORT void SwapWindow();
        DLL_EXPORT void ClearBufferBits();
        DLL_EXPORT int Close();
        DLL_EXPORT void SetClearBufferBits(int bits);
        DLL_EXPORT int WindowWidth();
        DLL_EXPORT int WindowHeight();
    private:
        enum error_code
        {
            ERROR_NO_WINDOW = 1,
            ERROR_NO_CONTEXT = 2,
            ERROR_GLEW_INIT_FAILED = 4,
        };
        int m_WindowWidth;
        int m_WindowHeight;
        int CLEAR_BUFFER_BITS = 0;
        SDL_Window* m_SDLWindow;
    };


}