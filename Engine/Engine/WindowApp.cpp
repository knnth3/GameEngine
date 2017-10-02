//#include "WindowApp.h"
//
//namespace Lime
//{
//    WindowApp::WindowApp()
//    {
//        m_SDLWindow = nullptr;
//    }
//    int WindowApp::Initialize(std::string windowName, int windownWidth, int windowHeight, unsigned int flags)
//    {
//        Uint32 Currentflags = SDL_WINDOW_OPENGL;
//        Currentflags |= flags;
//        int errorCode = 1;
//
//        SDL_Init(SDL_INIT_EVERYTHING);
//
//        m_SDLWindow = SDL_CreateWindow
//        (
//            windowName.c_str(),
//            SDL_WINDOWPOS_CENTERED,
//            SDL_WINDOWPOS_CENTERED,
//            windownWidth, windowHeight,
//            Currentflags
//        );
//
//        if (m_SDLWindow == nullptr)
//            errorCode = errorCode | ERROR_NO_WINDOW;
//
//        SDL_GLContext glContext = SDL_GL_CreateContext(m_SDLWindow);
//        if (glContext == nullptr)
//            errorCode = errorCode | ERROR_NO_CONTEXT;
//
//        GLenum error = glewInit();
//        if (error != GLEW_OK)
//            errorCode = errorCode | ERROR_GLEW_INIT_FAILED;
//
//        //checks opengl version
//        std::printf("** OpenGL Version: %s **\n", glGetString(GL_VERSION));
//
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//        //v-sync
//        SDL_GL_SetSwapInterval(1);
//
//
//
//        return errorCode;
//    }
//
//    void WindowApp::SwapWindow()
//    {
//        SDL_GL_SwapWindow(m_SDLWindow);
//        SDL_GetWindowSize(m_SDLWindow, &m_WindowWidth, &m_WindowHeight);
//    }
//
//    void WindowApp::ClearBufferBits()
//    {
//        if (CLEAR_BUFFER_BITS != 0)
//            glClear(CLEAR_BUFFER_BITS);
//    }
//
//    int WindowApp::Close()
//    {
//        SDL_DestroyWindow(m_SDLWindow);
//        SDL_Quit();
//        return 0;
//    }
//
//    void WindowApp::SetClearBufferBits(int bits)
//    {
//        CLEAR_BUFFER_BITS |= bits;
//    }
//
//    int WindowApp::WindowWidth()
//    {
//        return m_WindowWidth;
//    }
//
//    int WindowApp::WindowHeight()
//    {
//        return m_WindowHeight;
//    }
//
//}