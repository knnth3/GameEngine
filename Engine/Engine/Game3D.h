#pragma once
#include <string>
#include "WindowApp.h"
#include "SpriteBatch.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "Camera.h"
#include "SkyBox.h"

namespace Lime
{

    class Game3D
    {
    public:
        Game3D(const std::string gameTitle);
        void GameLoop();
    private:
        void Close();
        void Initialize();
        void SwapWindow();
        void SetActive();
        void ClearBufferBits();

        std::shared_ptr<unsigned int> m_WindowWidth;
        std::shared_ptr<unsigned int> m_WindowHeight;
        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<int> x_pos;
        std::shared_ptr<int> y_pos;
        std::shared_ptr<int> x_posCam;
        std::shared_ptr<int> y_posCam;
        std::shared_ptr<int> z_posCam;
        std::string m_WindowName;
        WindowApp m_Window;
        bool active = true;
        SpriteBatch sb;
        ShaderProgram sp;
        InputManager m_Input;
        SkyBox m_skybox;
        std::shared_ptr<Model> model1;
    };
}