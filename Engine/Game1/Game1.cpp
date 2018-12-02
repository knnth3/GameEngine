#include "Game1.h"
#include "Engine\MeshLoader.h"
#include "GameServer.h"
#include "GameClient.h"

using namespace std;
using namespace Engine;


Game1::Game1(std::string appName, float width, float height) :
	GraphicsWindow(appName, width, height)
{
	m_skybox.Scale(10000, 10000, 10000);
	m_skybox.SetDrawStyle(DRAW_STYLE_CUBEMAP | DRAW_STYLE_CULL_FRONT);
	m_gametype = GAMETYPE_RUNNING;

	m_mainMenu.SetDimensions(width, height);
}

void Game1::Update()
{
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto timer = WindowResources::GetTimer();
	double elapsed = timer->elapsed();

	//Skybox
	m_skybox.SetPosition(camera->GetPosition());
	HandleBasicControls(elapsed);
	SetUpGamestate();

	if(m_gamestate)
		m_gamestate->Update(elapsed);
}

void Game1::Render(const EGraphics& graphics)
{
	graphics->Draw(m_skybox);

	if (m_gamestate)
	{
		m_gamestate->Render(graphics);
	}
	else
	{
		graphics->Draw(m_mainMenu);
	}
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

	ToggleConsole();
	auto console = WindowResources::GetConsole();
	console->Log("This is the console. To open/close press 'N'.");
	console->Log("Controls: WASD camera movement, QE zoom keys, 'L' toggles wireframe,");
	console->Log("'P' toggles fullscreen.");

	std::wstring error;
	m_bgImageID = BrushManager::CreateNewImage("Assets/textures/MainMenu.png", error);
	m_mainMenu.SetBrush(m_bgImageID, BRUSH_TEXTURE_IMAGE);
}

void Game1::Suspend()
{
	if(m_gamestate)
		m_gamestate->Close();

	BrushManager::DeleteImage(m_bgImageID);
}

void Game1::HandleBasicControls(double elapsed)
{
	auto intput = WindowResources::GetInput();
	if (intput->GetKeyboard()->ButtonPressed('P'))
	{
		ToggleFullscreen();
	}
	if (intput->GetKeyboard()->ButtonPressed('L'))
	{
		WindowResources::GetGraphics()->ToggleWireframe();
	}
	if (intput->GetKeyboard()->ButtonPressed('N'))
	{
		ToggleConsole();
	}
	if (intput->GetKeyboard()->ButtonPressed('Y'))
	{
		m_gametype = GAMETYPE_SERVER;
	}
	if (intput->GetKeyboard()->ButtonPressed('T'))
	{
		m_gametype = GAMETYPE_CLIENT;
	}
	if (m_gamestate)
	{
		m_gamestate->HandleControlls(elapsed, intput, WindowResources::GetGraphics()->GetCamera());
	}
}

void Game1::SetUpGamestate()
{
	switch (m_gametype)
	{
	case Game1::GAMETYPE_SERVER:
		m_gamestate = std::make_unique<GameServer>(1234);
		m_gamestate->Initialize();
		m_gametype = GAMETYPE_RUNNING;
		break;
	case Game1::GAMETYPE_CLIENT:
		m_gamestate = std::make_unique<GameClient>("127.0.0.1", 1234);
		m_gamestate->Initialize();
		m_gametype = GAMETYPE_RUNNING;
		break;
	default:
		break;
	}
}