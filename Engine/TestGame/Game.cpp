#include "Game.h"
#include <Windows.h>
using namespace Lime::Model;

Game::Game() :
	App(GRAPHICS_API::DIRECTX_11)
{
}

void Game::OnInitialize()
{

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
		//m_fps = (int)(1.0f / time.count());
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

}

void Game::Update(float elapsed)
{
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
	m_renderBatch->Draw();
}

void Game::Clear()
{
	m_renderBatch->ClearScreen(glm::vec3(red, green, blue));
}
