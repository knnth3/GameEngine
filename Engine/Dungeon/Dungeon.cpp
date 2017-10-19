#include "Dungeon.h"
#include "GS_MapEditor.h"



Dungeon::Dungeon()
{
}

Dungeon::~Dungeon()
{
}

void Dungeon::Tick()
{
	static int count = 0;
	count++;
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
	if (count >= 1000)
	{
		m_fps = (int)(1.0f / time.count());
		count = 0;
	}
	Update(time.count());

	Render();
}

void Dungeon::OnInitialize()
{
	UINT width, height;
	GetWindowSize(width, height);
	m_camera = std::make_shared<Lime::Camera>();
	m_camera->Initialize(width, height);
	m_graphicsDevice->AttatchCamera(m_camera);
	m_input->LoadCamera(m_camera);
	start = std::chrono::system_clock::now();
	end = start;
	m_state = std::make_unique<GameStates::MainMenu>(m_graphicsDevice);
	m_state->Initialize(width, height);
}

void Dungeon::OnActivated()
{
}

void Dungeon::OnDeactivated()
{
}

void Dungeon::OnSuspending()
{
}

void Dungeon::OnResuming()
{
}

void Dungeon::OnWindowMoved()
{
}

void Dungeon::OnWindowSizeChanged(int width, int height)
{
	m_graphicsDevice->ResizeWindow(width, height);
	m_state->OnWindowResize(width, height);
}

void Dungeon::Update(float elapsed)
{
	using namespace GameStates;
	States result = m_state->Update(elapsed, m_input);

	//Gamestate changes
	switch (result)
	{
	case GameStates::CLOSE:
		exit(0);
		break;
	case GameStates::RUNNING:
		break;
	case GameStates::MAIN_MENU:
	{
		m_state.release();
		m_graphicsDevice->Reset();
		m_state = std::make_unique<MainMenu>(m_graphicsDevice);
		UINT width, height;
		GetWindowSize(width, height);
		m_state->Initialize(width, height);
	}
		break;
	case GameStates::MAP_EDITOR:
	{
		m_state.release();
		Lime::TextureManager::Clear();
		m_graphicsDevice->Reset();
		m_state = std::make_unique<MapEditor>(m_graphicsDevice, m_camera);
		UINT width, height;
		GetWindowSize(width, height);
		m_state->Initialize(width, height);
	}
		break;
	default:
		break;
	}
}

void Dungeon::Render()
{
	Clear();

	m_graphicsDevice->Draw();
}

void Dungeon::Clear()
{
	m_graphicsDevice->ClearScreen(glm::vec3(0.3f, 0.3f, 0.7f));
}
