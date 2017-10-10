#pragma once

#include <string>
#include <SDL2\SDL.h>
#include <gl\glew.h>
#include "Shared.h"

namespace Lime
{

    class WindowApp
    {
    public:
        AppDLL_API WindowApp();
        //Returns true if started correctly if else, returns an error code number for SDL
        AppDLL_API int Initialize(std::string windowName, int windownWidth, int windowHeight, unsigned int flags = 0);
        AppDLL_API void SwapWindow();
        AppDLL_API void ClearBufferBits();
        AppDLL_API int Close();
        AppDLL_API void SetClearBufferBits(int bits);
        AppDLL_API int WindowWidth();
        AppDLL_API int WindowHeight();
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