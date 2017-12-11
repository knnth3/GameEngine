#include "pch.h"
#include "GS_SceneManager.h"
#include <Lime_Engine\EngineResources.h>
#include <Graphics\BrushManager.h>

using namespace Graphics;
using namespace LIME_ENGINE;
using namespace Windows::System;
using namespace std;


bool GameStates::SceneManager::CreateDeviceIndependentResources()
{
	m_pathFinder.SetNodeDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	m_ground.SetPosition(0.0f, 0.0f);
	return true;
}

void GameStates::SceneManager::CreateDeviceDependentResources()
{
	std::wstring err;
	m_spriteSheetID = BrushManager::CreateNewImage("Assets/textures/Tileset/spritesheet.png", err);
	auto bg = BrushManager::CreateNewImage("Assets/textures/light_tile.jpg", err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);
	auto brush = BrushManager::CreateNewImageBrush(bg, err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);

	m_ground.SetBrush(brush, BRUSH_TEXTURE_IMAGE_BRUSH);

	m_effect = make_shared<Effects::SpotLightDiffuse>();
	BrushManager::AddEffect(m_effect);

	LoadScene();
	BeginNewScene();
	OnWindowResize();
}

void GameStates::SceneManager::Close()
{
	ShutDownScene();
	BrushManager::ClearEffect();
}

void GameStates::SceneManager::Draw()
{
	auto graphics = EngineResources::GetGraphicsDevice();
	graphics->Draw(m_ground, true);

	for(auto& actor : m_actors)
		graphics->Draw(actor.Get());

	for (auto& line : m_pathLines)
		graphics->Draw(line);
}

GameStates::States GameStates::SceneManager::Update()
{
	static int lineBrush = BrushManager::CreateNewBrush(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_actors[0].Update();
	m_actors[0].GetDirection();
	UpdateLighting();
	auto input = EngineResources::GetInputManager();
	auto timer = EngineResources::GetTimer();
	float elapsed = (float)timer->GetElapsedSeconds();
	if (input->KeyPressed((int)VirtualKey::Escape))
	{
		return MAIN_MENU;
	}
	if (input->LMBPressed())
	{
		auto mouse = input->GetMouseCoords();
		int graphX = (int)floorf(mouse.x / BLOCK_WIDTH);
		int graphY = (int)floorf(mouse.y / BLOCK_HEIGHT);

		NodePositions path;
		auto start = m_actors[0].GetPosition();
		m_pathFinder.SetStartPoint((int)start.x, (int)start.y);
		m_pathFinder.SetEndPoint(graphX, graphY);
		if (m_pathFinder.FindPath(path))
		{
			m_actors[0].SetMoveInstructions(path);
			Line l;
			l.SetBrush(lineBrush);
			m_pathLines.clear();
			float xOffset = BLOCK_WIDTH * 0.5f;
			float yOffset = BLOCK_HEIGHT * 0.5f;
			for (size_t x = 0; x < path.size() - 1; x++)
			{
				auto& point1 = path[x];
				auto& point2 = path[x + 1];
				l.SetPointOne(D2D1::Point2F((point1.x * BLOCK_WIDTH) + xOffset, (point1.y * BLOCK_WIDTH) + yOffset));
				l.SetPointTwo(D2D1::Point2F((point2.x * BLOCK_HEIGHT) + xOffset, (point2.y * BLOCK_HEIGHT) + yOffset));
				m_pathLines.push_back(l);
			}
		}
	}
	if (input->LMBStatus())
	{
	}
	return RUNNING;
}

void GameStates::SceneManager::OnWindowResize()
{
	auto graphics = EngineResources::GetGraphicsDevice();

	float width, height;
	graphics->GetWindowDimensions(width, height);

	int graphX = (int)roundf(width / BLOCK_WIDTH);
	int graphY = (int)roundf(height / BLOCK_HEIGHT);
	m_pathFinder.SetGridDimensions(graphX, graphY);

	m_ground.SetDimensions(width, height);
}

void GameStates::SceneManager::UpdateLighting()
{
	auto input = EngineResources::GetInputManager();
	auto pos = m_actors[0].GetModelPosition();
	m_effect->SetLightPosition(pos);
	m_effect->SetFocusPosition(pos);
}

bool GameStates::SceneManager::LoadScene()
{
	std::wstring err;
	static int bg = BrushManager::CreateNewImage("Assets/textures/Basic_Character/rpg_sprite_walk.png", err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);

	Actor a;
	a.SetPosition(3.0f, 5.0f);
	a.SetDimensions(48.0f, 72.0f);
	a.SetBrush(bg, BRUSH_TEXTURE_IMAGE);
	a.SetImageAtlasDivisons(8, 4);
	m_actors.push_back(a);

	Actor b;
	b.SetPosition(4.0f, 5.0f);
	b.SetDimensions(64.0f, 64.0f);
	b.SetBrush(m_spriteSheetID, BRUSH_TEXTURE_IMAGE);
	b.SetImageAtlasPosition(1, 1);
	b.SetImageAtlasDivisons(64, 95);
	m_actors.push_back(b);
	return true;
}

bool GameStates::SceneManager::BeginNewScene()
{
	return false;
}

void GameStates:: SceneManager::ShutDownScene()
{
	m_actors.clear();
}
