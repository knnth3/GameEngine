#include "WindowApp.h"
#include "Engine\MeshLoader.h"

using namespace std;
using namespace Engine;


WindowApp::WindowApp(std::string appName, float width, float height):
	GraphicsWindow(appName, width, height)
{
	m_cube1.Scale(20, 20, 20);
	m_cube1.SetColor(0.0f, 0.0f, 1.0f);

	m_cube2.Scale(10000, 10000, 10000);
	m_cube2.SetDrawStyle(DRAW_STYLE_CUBEMAP | DRAW_STYLE_CULL_FRONT);

	t = "Loading...";
	t.SetBounds(800, 30);
}

void WindowApp::Update()
{
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto timer = WindowResources::GetTimer();
	//Color
	static bool wf = false;
	static int red = 0;
	static int green = 0;
	static int blue = 0;
	static int rrel = 1;
	static int grel = 2;
	static int brel = 4;
	red += rrel;
	green += grel;
	blue += brel;
	if (red > 255 || red < 0)
		rrel = -rrel;
	if (green > 255 || green < 0)
		grel = -grel;
	if (blue > 255 || blue < 0)
		brel = -brel;
	float redf = (float)red / 255.0f;
	float greenf = (float)green / 255.0f;
	float bluef = (float)blue / 255.0f;
	m_cube1.SetColor(redf, greenf, bluef);

	//Skybox
	m_cube2.SetPosition(camera->GetPosition());

	//Rotation
	float speed = 2.45f;
	float seconds = (float)timer->elapsed() / 1000.0f;
	float rotation = speed * seconds;
	m_cube1.RotateRelative(0.0f, rotation, 0.0f);

	//Mouse
	auto input = WindowResources::GetInput()->GetMouse();
	glm::vec2 pos = input->GetPositon();

	//Keyboard
	auto keyboard = WindowResources::GetInput()->GetKeyboard();
	if (keyboard->ButtonDown('W'))
	{
		camera->Rotate(rotation, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('S'))
	{
		camera->Rotate(-rotation, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('A'))
	{
		camera->Rotate(0.0f, rotation, 0.0f);
	}
	if (keyboard->ButtonDown('D'))
	{
		camera->Rotate(0.0f, -rotation, 0.0f);
	}
	if (keyboard->ButtonPressed('P'))
	{
		ToggleFullscreen();
	}
}

void WindowApp::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_cube1);
	graphics->Draw(m_cube2);
	graphics->Draw(m_floor);
	graphics->Draw(t);
}

void WindowApp::Resume()
{
	auto graphics = WindowResources::GetGraphics();
	t = graphics->GetVideoCardInfo().at(0).name;
	Skybox sb;
	sb.path = "Assets/textures/Default/cubemap.dds";
	graphics->SetSkybox(sb);

	//int mesh = MeshLoader::CreatePlane(100, 100, 10, 10);
	int mesh = MeshLoader::LoadModel("Assets/models/body_robe_bronze_common.bin");
	m_floor.SetMesh(mesh);
	m_floor.Scale(100, 100, 100);
}

void WindowApp::Suspend()
{
}
