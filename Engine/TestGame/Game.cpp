#include "Game.h"
#include <Windows.h>

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

	model = std::make_shared<Model3D>();
	model2 = std::make_shared<Model3D>();
	model->m_Data->m_Verticies =
	{
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f),
		Vertex(-1.0f, +1.0f, -1.0f, 1.0f, 1.0f, -1.0f, +1.0f, -1.0f),
		Vertex(+1.0f, +1.0f, -1.0f, 1.0f, 1.0f, +1.0f, +1.0f, -1.0f),
		Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, +1.0f, -1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, +1.0f, 1.0f, 1.0f, -1.0f, -1.0f, +1.0f),
		Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, -1.0f, +1.0f, +1.0f),
		Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 1.0f, +1.0f, +1.0f, +1.0f),
		Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 1.0f, +1.0f, -1.0f, +1.0f),
	};
	model->m_Data->m_Indicies = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	model2->m_Data = model->m_Data;
	m_camera->AttachToModel(model2);
	model->Color(1.0f, 0.0f, 1.0f, 0.5f);
	model->Translate(0.0f, 0.0f, -5.0f);
	Texture tex = m_graphicsDevice->LoadTextureFromFile(L"images.dds");
	model->SetTexture(tex);
	model2->SetTexture(tex);
	m_graphicsDevice->DrawModel(model);
	m_graphicsDevice->DrawModel(model2);
	m_graphicsDevice->DrawText("Loading...", controller);
	controller->Position(glm::vec3(0.0f, 3.0f, 0.0f));
	controller->Scale(glm::vec3(0.25f, 0.25f, 0.25f));
	controller->Color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//m_graphicsDevice->Wireframe(true);
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
	rot += 9.8f / 20.0f * elapsed;
	if (rot > 6.28f)
		rot = 0.0f;
	//model2->Rotate(0.0f, -rot, 0.0f);
	model->RotateAtOrigin(0.0f, rot, 0.0f);

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
	//Clear our backbuffer to the updated color
	auto context = m_graphicsDevice->GetDeviceContext();
	auto rtv = m_graphicsDevice->GetRenderTargetView();
	auto depthStencil = m_graphicsDevice->GetDepthStencilView();

	const float bgColor[4] = { red, green, blue, 1.0f };

	context->ClearRenderTargetView(rtv, bgColor);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, depthStencil);

	auto viewport = m_graphicsDevice->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
}
