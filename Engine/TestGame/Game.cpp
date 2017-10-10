#include "Game.h"
#include <Windows.h>
using namespace Lime::Model;

Game::Game() :
	App()
{
}

void Game::OnInitialize()
{
	UINT width, height;
	GetDefaultSize(width, height);
	m_camera = std::make_shared<Lime::Camera>();
	m_camera->Initialize(width, height);
	m_graphicsDevice->AttatchCamera(m_camera);
	start = std::chrono::system_clock::now();
	end = start;

	std::vector<Vertex> m_verties = 
	{
		{ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(300.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.0f, -300.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(300.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(300.0f, -300.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.0f, -300.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }
	};
	std::vector<uint32_t> m_indices =
	{
		0,1,2,
		3,4,5
	};


	auto mesh1 = Model::MeshLoader::LoadModel("Cube_TextureWrap.fbx");
	auto mesh2 = Model::MeshLoader::LoadModel(m_verties, m_indices);
	model1 = std::make_shared<Model3D>(mesh1);
	model2 = std::make_shared<Model3D>(mesh1);
	model3 = std::make_shared<Model3D>(mesh2);
	model3->SetType(Model::MeshType::TWO_DIMENSION);
	m_camera->AttachToModel(model2);
	model1->SetPosition(0.0f, 3.0f, -15.0f);
	m_graphicsDevice->AddModel(model1);
	m_graphicsDevice->AddModel(model2);
	m_graphicsDevice->AddModel(model3);
	model2->Scale(0.25f, 0.25f, 0.25f);
	m_graphicsDevice->AddText("Loading...", controller);
	controller->Position(glm::vec3(0.0f, 3.0f, 0.0f));
	controller->Scale(glm::vec3(0.25f, 0.25f, 0.25f));
	controller->Color(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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
		controller->SetText("Fps: " + std::to_string(fps));
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

	model1->RotateAtOrigin(0.0f, rot, 0.0f);
	model2->Rotate(0.0f, -rot , 0.0f);

	model1->SetColor(red,green, blue);
	model2->SetColor(green, red, blue);

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
	m_graphicsDevice->ClearScreen(glm::vec3(0.0f, 0.0f, 0.0f));
}
