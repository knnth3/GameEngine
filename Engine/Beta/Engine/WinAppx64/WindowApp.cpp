#include "WindowApp.h"
#include "Engine\MeshLoader.h"

using namespace std;
using namespace Engine;


WindowApp::WindowApp(std::string appName, float width, float height):
	GraphicsWindow(appName, width, height)
{
	m_cube1.Scale(20, 20, 20);
	//m_cube1.SetColor(0.0f, 0.0f, 1.0f);

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
	//static bool wf = false;
	//static int red = 0;
	//static int green = 0;
	//static int blue = 0;
	//static int rrel = 1;
	//static int grel = 2;
	//static int brel = 4;
	//red += rrel;
	//green += grel;
	//blue += brel;
	//if (red > 255 || red < 0)
	//	rrel = -rrel;
	//if (green > 255 || green < 0)
	//	grel = -grel;
	//if (blue > 255 || blue < 0)
	//	brel = -brel;
	//float redf = (float)red / 255.0f;
	//float greenf = (float)green / 255.0f;
	//float bluef = (float)blue / 255.0f;
	//m_cube1.SetColor(redf, greenf, bluef);

	//Skybox
	m_cube2.SetPosition(camera->GetPosition());

	//Rotation
	float acceleration = 2.45f;
	double seconds = timer->elapsed() / 1000.0;
	float velocity = float(acceleration * seconds);
	m_model.Update(seconds);
	m_cube1.RotateRelative(0.0f, velocity, 0.0f);
	//m_model.RotateRelative(0.0f, rotation, 0.0f);

	//Mouse
	auto input = WindowResources::GetInput()->GetMouse();
	glm::vec2 pos = input->GetPositon();

	//Keyboard
	auto keyboard = WindowResources::GetInput()->GetKeyboard();
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
		m_model.ToggleAnimation();
	}
}

void WindowApp::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_cube1);
	graphics->Draw(m_cube2);
	graphics->Draw(m_model);
	graphics->Draw(t);
}

void WindowApp::Resume()
{
	auto graphics = WindowResources::GetGraphics();
	graphics->GetCamera()->EnforceBounds(false);
	t = graphics->GetVideoCardInfo().at(0).name;
	Skybox sb;
	sb.path = "Assets/textures/Default/cubemap.dds";
	graphics->SetSkybox(sb);

	//int mesh = MeshLoader::CreatePlane(100, 100, 10, 10);
	int mesh = MeshLoader::LoadModel("Assets/models/body_plate_silver_epic_rest1.sef");
	int mesh2 = MeshLoader::LoadModel("Assets/models/head1.sef");
	m_model.SetMesh(mesh);
	m_model.SetSecondaryMesh(mesh2);
	m_model.Scale(100, 100, 100);
	m_model.ToggleAnimation();
	//m_model.SetColor(2.0f, 0.44f, 0.0f);
}

void WindowApp::Suspend()
{
}
