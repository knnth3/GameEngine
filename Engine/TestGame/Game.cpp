#include "Game.h"


Game::Game() :
	DX11App()
{
}

void Game::Initialize()
{
	int width, height;
	model = std::make_shared<Model2>();
	model->m_Data->m_Verticies =
	{
		// Front Face
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex2(-1.0f, -1.0f, 1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, 1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex2(-1.0f, 1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f, 1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,1.0f, 0.0f),
		Vertex2(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,1.0f, 1.0f),

		// Bottom Face
		Vertex2(-1.0f, -1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f, -1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex2(-1.0f, -1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(-1.0f, -1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex2(1.0f, -1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, -1.0f,  1.0f,0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
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
	model->Color(1.0f, 0.0f, 1.0f);
	model->Translate(3.0f, 0.0f, -3.0f);
	m_graphicsDevice->AddModel(model);
	m_graphicsDevice->LoadTextureFromFile(L"images.dds");

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
}

void Game::Update(float elapsed)
{
	rot += 9.8 / 20.0f * elapsed;
	if (rot > 6.28f)
		rot = 0.0f;

	model->Rotate(rot, rot, rot);
	model->RotateAtOrigin(rot, rot, rot);
}

void Game::Render()
{
	Clear();

	m_graphicsDevice->Draw();
}

void Game::Clear()
{
}
