#include "Game.h"
#include <Windows.h>
using namespace Lime::Model;

Game::Game() :
	DX11App()
{
}

void Game::Initialize()
{
	UINT width, height;
	GetDefaultSize(width, height);
	m_camera = std::make_shared<Lime::Camera>();
	m_camera->Initialize(width, height);
	m_graphicsDevice->AttatchCamera(m_camera);
	start = std::chrono::system_clock::now();
	end = start;

	auto mesh = Model::MeshLoader::LoadModel("Cube_HardEdges.fbx");
	model = std::make_shared<Model3D>(mesh);
	model2 = std::make_shared<Model3D>(mesh);
	m_camera->AttachToModel(model2);
	model->SetColor(1.0f, 0.0f, 1.0f, 0.5f);
	model->SetPosition(0.0f, 3.0f, -15.0f);
	//m_graphicsDevice->AddModel(model);
	m_graphicsDevice->DrawModel(model2);
	model2->Scale(0.5f, 0.5f, 0.5f);
	m_graphicsDevice->DrawText("Loading...", controller);
	//controller->Position(glm::vec3(0.0f, 3.0f, 0.0f));
	//controller->Scale(glm::vec3(0.25f, 0.25f, 0.25f));
	//controller->Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Game::Tick()
{
	static int count = 0;
	count++;
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
	if (count >= 1000)
	{
		int fps = (int)(1.0f / time.count());
		//controller->SetText("Fps: " + std::to_string(fps));
		count = 0;
	}
	Update(time.count());

	Render();
}

void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
}

void Game::OnResuming()
{
}

void Game::OnWindowMoved()
{
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_graphicsDevice->ResizeWindow(width, height);
}

void Game::Update(float elapsed)
{
	camRot = 9.8f / 2.0f * elapsed;
	float camRotY = camRot;
	if (camRotY >= 3.14f)
		camRotY = -camRotY;
	if (m_input->KeyStatus(0x41) == true) //A
	{
		m_camera->Rotate(camRot, 0.0f, 0.0f);
	}
	if (m_input->KeyStatus(0x44) == true) //D
	{
		m_camera->Rotate(-camRot, 0.0f, 0.0f);
	}
	if (m_input->KeyStatus(0x57) == true) //W
	{
		m_camera->Rotate(0.0f, camRot, 0.0f);
	}
	if (m_input->KeyStatus(0x53) == true) //S
	{
		m_camera->Rotate(0.0f, -camRot, 0.0f);
	}
	if (m_input->KeyStatus(0x51) == true) //Q
	{
		m_camera->Zoom(-camRot);
	}
	if (m_input->KeyStatus(0x45) == true) //E
	{
		m_camera->Zoom(camRot);
	}
	rot += 9.8f / 20.0f * elapsed;
	if (rot > 6.28f)
		rot = 0.0f;
	//model2->Rotate(0.0f, -rot/2.0f, 0.0f);
	//model->RotateAtOrigin(0.0f, rot, 0.0f);

	//Update the colors of our scene
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
}

void Game::Render()
{
	Clear();

	m_graphicsDevice->Draw();
}

void Game::Clear()
{
	m_graphicsDevice->ClearScreen(glm::vec3(red, green, blue));
}
