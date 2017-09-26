#include "Game.h"
#include <Windows.h>

Game::Game() :
	DX11App()
{
}

void Game::Initialize()
{
	char c = 'B';
	int letter = (c - 31) + 5;
	int posy;
	int posx = letter % 20;
	if (posx == 0)
	{
		posx = letter;
		posy = letter / 20;
	}
	else
	{
		posy = (letter / 20) + 1;
	}
	float xLen = 0.05f;
	float yLen = 0.2f;
	float row = (float)posx;
	float collumn = (float)posy;
	float maxX = xLen*row;
	float maxY = yLen*collumn;
	float minX = maxX - xLen;
	float minY = maxY - yLen;
	int width, height;
	model = std::make_shared<Model2>();
	model2 = std::make_shared<Model2>();
	model->m_Data->m_Verticies =
	{
		// Front Face
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f, -1.0f,  1.0f, 0.0f),
		Vertex2(1.0f, -1.0f, -1.0f,  1.0f, 1.0f),

		// Back Face
		Vertex2(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex2(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex2(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex2(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex2(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex2(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex2(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex2(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};
	model->m_Data->m_Indicies = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	model2->m_Data = model->m_Data;
	model->Color(1.0f, 0.0f, 1.0f, 0.5f);
	model->SetOpacity(0.5f);
	model->Translate(3.0f, 0.0f, -3.0f);
	LPCWSTR m_vsPath = L"shaders/VertexShader.hlsl";
	LPCWSTR m_psPath = L"shaders/PixelShader.hlsl";
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	m_graphicsDevice->CreateShaders(m_vsPath, m_psPath, layout, 2);
	m_vsPath = L"shaders/FontVertexShader.hlsl";
	m_psPath = L"shaders/FontPixelShader.hlsl";
	m_graphicsDevice->CreateShaders(m_vsPath, m_psPath, layout, 2);
	m_graphicsDevice->AddModel(model);
	m_graphicsDevice->AddModel(model2);
	std::shared_ptr<Lime::TextController> controller = nullptr;
	m_graphicsDevice->CreateTextObject("Hello World!", controller);
	controller->Position(glm::vec3(0.0f, 3.0f, 0.0f));
	controller->Scale(glm::vec3(0.25f, 0.25f, 0.25f));
	m_graphicsDevice->LoadTextureFromFile(L"images.dds");
	m_graphicsDevice->LoadTextureFromFile(L"SpriteSheetx200.dds");
	GetDefaultSize(width, height);
	auto camera = std::make_shared<Lime::DX11Camera>(width, height);
	m_graphicsDevice->AttatchCamera(camera);

	start = std::chrono::system_clock::now();
	end = start;
}

void Game::Tick()
{
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
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
	rot += 9.8 / 20.0f * elapsed;
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
