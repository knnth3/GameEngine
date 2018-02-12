#include "Game1.h"
#include "Engine\MeshLoader.h"

using namespace std;
using namespace Engine;


Game1::Game1(std::string appName, float width, float height) :
	GraphicsWindow(appName, width, height)
{
	m_skybox.Scale(10000, 10000, 10000);
	m_skybox.SetDrawStyle(DRAW_STYLE_CUBEMAP | DRAW_STYLE_CULL_FRONT);
}

void Game1::Update()
{
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto timer = WindowResources::GetTimer();
	double elapsed = timer->elapsed();

	//Skybox
	m_skybox.SetPosition(camera->GetPosition());
	HandleBasicControls(elapsed);
	m_gamestate->Update(elapsed);
}

void Game1::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_skybox);
	m_gamestate->Render(graphics);
}

void Game1::Resume()
{
	auto graphics = WindowResources::GetGraphics();
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto mouse = WindowResources::GetInput()->GetMouse();
	mouse->AttatchCamera(camera);
	Skybox sb;
	sb.path = "Assets/textures/Default/cubemap.dds";
	graphics->SetSkybox(sb);
	m_gamestate = std::make_unique<GameState>();
	m_gamestate->Initialize();

	if (_DEBUG)
	{
		ToggleConsole();
		auto console = WindowResources::GetConsole();
		console->Log("This is the console. To open/close press 'N'.");
		console->Log("Controls: WASD camera movement, QE zoom keys, 'L' toggles wireframe,");
		console->Log("'P' toggles fullscreen.");
	}
}

void Game1::Suspend()
{
	m_gamestate->Close();
}

void Game1::HandleBasicControls(double elapsed)
{
	//Keyboard
	auto keyboard = WindowResources::GetInput()->GetKeyboard();
	auto mouse = WindowResources::GetInput()->GetMouse();
	auto camera = WindowResources::GetGraphics()->GetCamera();

	//Rotation
	float acceleration = 2.45f;
	double seconds = elapsed / 1000.0;
	float velocity = float(acceleration * seconds);

	if (keyboard->ButtonDown('W'))
	{
		camera->Rotate(velocity, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('S'))
	{
		camera->Rotate(-velocity, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('A'))
	{
		camera->Rotate(0.0f, velocity, 0.0f);
	}
	if (keyboard->ButtonDown('D'))
	{
		camera->Rotate(0.0f, -velocity, 0.0f);
	}
	if (keyboard->ButtonDown('Q'))
	{
		camera->Zoom(velocity * 100.0f);
	}
	if (keyboard->ButtonDown('E'))
	{
		camera->Zoom(-velocity * 100.0f);
	}
	if (keyboard->ButtonPressed('P'))
	{
		ToggleFullscreen();
	}
	if (keyboard->ButtonPressed('L'))
	{
		WindowResources::GetGraphics()->ToggleWireframe();
	}
	if (keyboard->ButtonPressed('N'))
	{
		ToggleConsole();
	}
}
