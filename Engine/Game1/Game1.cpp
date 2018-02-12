#include "Game1.h"
#include "Engine\MeshLoader.h"

using namespace std;
using namespace Engine;


Game1::Game1(std::string appName, float width, float height) :
	GraphicsWindow(appName, width, height)
{
	m_cube1.Scale(100, 100, 100);
	//m_cube1.SetColor(0.0f, 0.0f, 1.0f);

	m_cube2.Scale(10000, 10000, 10000);
	m_cube2.SetDrawStyle(DRAW_STYLE_CUBEMAP | DRAW_STYLE_CULL_FRONT);

	t = "Loading...";
	t.SetBounds(800, 30);
}

void Game1::Update()
{
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto timer = WindowResources::GetTimer();

	//Skybox
	m_cube2.SetPosition(camera->GetPosition());

	//Rotation
	float acceleration = 2.45f;
	double seconds = timer->elapsed() / 1000.0;
	float velocity = float(acceleration * seconds);
	m_model.Update(seconds);

	//Mouse
	auto input = WindowResources::GetInput()->GetMouse();
	glm::vec2 pos = input->GetPositon();

	//Keyboard
	auto keyboard = WindowResources::GetInput()->GetKeyboard();
	auto mouse = WindowResources::GetInput()->GetMouse();
	mouse->AttatchCamera(camera);
	if (keyboard->ButtonDown(' '))
	{
		m_model.SetAnimation(2);
	}
	else if (mouse->ButtonDown(0))
	{
		m_model.SetAnimation(1);
		auto pos = mouse->Get3DPosition_2();
		m_model.SetPosition(pos);
		t = "Mouse button down";
	}
	else
	{
		m_model.SetAnimation(0);
	}
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

void Game1::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_model);
	graphics->Draw(m_cube1);
	graphics->Draw(m_cube2);
	graphics->Draw(t);
}

void Game1::Resume()
{
	auto graphics = WindowResources::GetGraphics();
	graphics->GetCamera()->EnforceBounds(false);
	t = graphics->GetVideoCardInfo().at(0).name;
	Skybox sb;
	sb.path = "Assets/textures/Default/cubemap.dds";
	graphics->SetSkybox(sb);

	//int mesh = MeshLoader::CreatePlane(100, 100, 10, 10);
	int mesh = MeshLoader::LoadModel("Assets/models/body_plate_silver_epic.sef");
	int mesh2 = MeshLoader::LoadModel("Assets/models/head1.sef");
	m_model.SetMesh(mesh);
	m_model.SetSecondaryMesh(mesh2);
	m_model.Scale(100, 100, 100);
	m_cube1 = m_model;
	m_cube1.SetPosition(300, 0, 0);
	m_model.SetAnimationDelay(2, true);
	//m_model.SetColor(2.0f, 0.44f, 0.0f);
}

void Game1::Suspend()
{
}
