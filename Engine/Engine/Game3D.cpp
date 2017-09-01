#include "Game3D.h"
#include "Primitives.h"
#include "ModelLoader.h"
#include "glm\gtc\matrix_transform.hpp"
#include "AudioManager.h"

namespace Lime
{


    Game3D::Game3D(const std::string gameTitle)
    {
        m_WindowName = gameTitle;
        m_WindowWidth = std::make_shared<unsigned int>(800);
        m_WindowHeight = std::make_shared<unsigned int>(600);
        m_Camera = std::make_shared<Camera>();
    }

    void Game3D::Initialize()
    {
        m_Window.Initialize(m_WindowName.c_str(), *m_WindowWidth, *m_WindowHeight, SDL_WINDOW_RESIZABLE);
        m_Camera->Initialize(m_WindowWidth, m_WindowHeight);
        sp.LoadShadersToProgram("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
        sb.Initialize(m_Camera, std::make_shared<ShaderProgram>(sp));
        m_Window.SetClearBufferBits(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        m_skybox.Initialize(m_Camera);
        model1 = std::make_shared<Model>();
        m_Input.LoadCamera(m_Camera);
    }

    void Game3D::SwapWindow()
    {
        m_Window.SwapWindow();
    }

    void Game3D::SetActive()
    {
        static bool worldMouse = false;
        bool close = m_Input.Update();
        if (close == false)
            active = false;
        if (m_Input.KeyStatus(SDLK_ESCAPE))
        {
            //worldMouse = !worldMouse;
            //switch(worldMouse)
            //{
            //case true:
            //    SDL_SetRelativeMouseMode(SDL_TRUE);
            //    break;
            //case false:
            //    SDL_SetRelativeMouseMode(SDL_FALSE);
            //    break;
            //};
        }
        float val = 0.025f;
        static float theta = 0.0f;
        if (m_Input.KeyStatus(SDLK_w))
        {
            m_Camera->AddPitch(val);
        }
        if (m_Input.KeyStatus(SDLK_s))
        {
            m_Camera->AddPitch(-val);
        }
        if (m_Input.KeyStatus(SDLK_d))
        {
            m_Camera->AddYaw(val);
        }
        if (m_Input.KeyStatus(SDLK_a))
        {
            m_Camera->AddYaw(-val);
        }
        if (m_Input.KeyStatus(SDLK_q))
        {
            m_Camera->ZoomIn(-val*10.f);
        }
        if (m_Input.KeyStatus(SDLK_e))
        {
            m_Camera->ZoomIn(val*10.f);
        }
    }

    void Game3D::ClearBufferBits()
    {
        m_Window.ClearBufferBits();
    }

    void Game3D::Close()
    {
        sp.DeleteProgram();
        sb.Close();
        m_skybox.Close();
        m_Window.Close();
    }

    void Game3D::GameLoop()
    {
        ////////////////////audio
        Initialize();
        AudioManager mAudioManager;
        bool b = mAudioManager.LoadWAV("audio/song.wav");
        std::cout << b << std::endl;
        if (b == 1)
        {
            mAudioManager.Play(true);
        }
        ////////////////////////////////////////////////////

        model1->m_Data = ModelLoader::CreateCube(1,1);
        model1->Translate(0, 1, 0);


        Model model2;
        model2.m_Data = ModelLoader::CreateCube(1,1);

        Model model3;
        model3.m_Data = ModelLoader::CreatePlane(1, 10, 10);

        m_Camera->AttachToModel(model1);
        while (active)
        {
            m_Camera->Update();
            ClearBufferBits();
            glViewport(0, 0, *m_WindowWidth, *m_WindowHeight);
            glm::vec3 mousePos = m_Input.GetMouse3DPosition2();

            model2.Translate(mousePos.x, mousePos.y + 1.0f, mousePos.z);
            m_skybox.Draw();
            sb.Begin();
            sb.Draw(*model1);
            sb.Draw(model2);
            sb.Draw(model3);
            sb.End();

            SetActive();
            SwapWindow();

            *m_WindowWidth = m_Window.WindowWidth();
            *m_WindowHeight = m_Window.WindowHeight();
        }
        mAudioManager.Close();
        Close();
    }
}