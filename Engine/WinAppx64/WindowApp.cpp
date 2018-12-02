#include "WindowApp.h"
#include "Engine\MeshLoader.h"

using namespace std;
using namespace Engine;


WindowApp::WindowApp(std::string appName, float width, float height):
	GraphicsWindow(appName, width, height)
{
	m_skybox.Scale(10000, 10000, 10000);
	m_skybox.SetDrawStyle(DRAW_STYLE_CUBEMAP | DRAW_STYLE_CULL_FRONT);
}

void WindowApp::Update()
{
	auto camera = WindowResources::GetGraphics()->GetCamera();
	auto timer = WindowResources::GetTimer();	
	double seconds = timer->elapsed() / 1000.0;
	m_model.Update(seconds);
}

void WindowApp::Render(const EGraphics& graphics)
{
	graphics->Draw(m_skybox);
	graphics->Draw(m_model);
}

void WindowApp::Resume()
{
	auto graphics = WindowResources::GetGraphics();
	graphics->GetCamera()->EnforceBounds(false);
	Skybox sb;
	sb.path = "Assets/textures/Default/cubemap.dds";
	graphics->SetSkybox(sb);

	//int mesh = MeshLoader::CreatePlane(100, 100, 10, 10);
	int mesh = MeshLoader::LoadModel("Assets/models/body_plate_silver_epic.sef");
	int mesh2 = MeshLoader::LoadModel("Assets/models/head1.sef");
	m_model.SetMesh(mesh);
	m_model.SetSecondaryMesh(mesh2);
	m_model.Scale(100, 100, 100);
	m_model.SetAnimationDelay(2, true);
}

void WindowApp::Suspend()
{
}
