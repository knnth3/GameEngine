#include "pch.h"
#include "TestGame.h"
#include <string>
#include <Graphics\TextureLoader.h>
#include <Graphics\MeshLoader.h>
#include <Lime_Engine\EngineResources.h>


using namespace LIME_ENGINE;
using namespace Windows::System;
using namespace Graphics;

TestGame::TestGame(std::shared_ptr<Graphics::GraphicsDevice>& graphicsDevice,
	std::shared_ptr<InputManager>& inputManager,
	std::shared_ptr<LIME_ENGINE::StepTimer>& timer)
{
	SetDefaultDimensions(832.0f, 640.0f);
	this->SetClearColor(0.2f, 0.2f, 0.2f);
}

void TestGame::OnUpdate()
{
	static float rotation = 0;
	auto timer = EngineResources::GetTimer();
	//Makes the block follow the mouse
	glm::vec3 mouseCoords;
	if (EngineResources::GetInputManager()->GetMouse3DPosition(mouseCoords))
		m_model2.SetPosition(mouseCoords);

	rotation += (float)timer->GetElapsedSeconds();
	m_model.Rotate(rotation, 0.0f, 0.0f);
}

void TestGame::OnRender()
{
	// Render the scene objects.
	// Order does matter for 2D objects
	auto graphics = EngineResources::GetGraphicsDevice();
	auto timer = EngineResources::GetTimer();
	Text t;
	t.SetPosition(0.0f, 0.0f);
	t.SetBounds(120.0f, 40.0f);
	uint32_t fps = timer->GetFramesPerSecond();
	t = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";
	graphics->Draw(t);
	graphics->Draw(m_line);
	graphics->Draw(m_model);
	graphics->Draw(m_model2);
}

void TestGame::OnResume()
{
	MeshID id = MeshLoader::LoadModel("Assets/Models/Sphere_smooth.obj");
	m_model2.SetMesh(id);
	m_model2.Scale(50.0f, 50.0f, 50.0f);
	m_model.Scale(100.0f, 100.0f, 100.0f);
	m_line.SetPointOne({ 0.0f, 0.0f });
	m_line.SetPointTwo({ 300.0f, 300.0f });
	EngineResources::GetGraphicsDevice()->GetCamera()->AttachToModel(m_model);
	EngineResources::GetConsole()->SetToggleButton(192);
	EngineResources::GetConsole()->Log(L"Hello!");
	EngineResources::GetConsole()->Log(L"How are you?");
	//this->GetGraphicsDevice()->Wireframe(true);
}

void TestGame::OnSuspend()
{
}

void TestGame::OnWindowSizeChanged()
{
}
