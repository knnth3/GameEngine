#include "pch.h"
#include "GS_MapEditor.h"
#include <Lime_Engine\EngineResources.h>
#include <Graphics\BrushManager.h>
//#include "DungeonMapTileset.h"

//Macros
#define BLOCK_WIDTH 32.0f
#define BLOCK_HEIGHT 32.0f

using namespace Graphics;
using namespace LIME_ENGINE;
using namespace Windows::System;
using namespace std;


//All initialization of complidated logic goes in here
bool GameStates::MapEditor::CreateDeviceIndependentResources()
{
	m_floor.SetPosition(0.0f, 0.0f);
	m_cursor.SetPosition(0.0f, 0.0f);
	m_cursor.SetDimensions(BLOCK_WIDTH, BLOCK_HEIGHT);
	return true;
}

void GameStates::MapEditor::CreateDeviceDependentResources()
{
	std::wstring err;
	auto bg = BrushManager::CreateNewImage("Assets/textures/grass.jpg", err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);
	auto brush = BrushManager::CreateNewImageBrush(bg, err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);

	m_floor.SetBrush(brush, BRUSH_TEXTURE_IMAGE);

	m_effect = make_shared<Effects::SpotLightDiffuse>();
	BrushManager::AddEffect(m_effect);

	OnWindowResize();
}

GameStates::States GameStates::MapEditor::Update()
{
	//static float camRot = 0.0f;
	//auto camera = EngineResources::GetGraphicsDevice()->GetCamera();
	auto input = EngineResources::GetInputManager();
	auto timer = EngineResources::GetTimer();
	float elapsed = (float)timer->GetElapsedSeconds();
	UpdateCursor(input->LMBPressed(), elapsed);




	static float focus = 0;
	static float coneDeg = 0;
	static float diffuse = 0;



	if (input->KeyStatus((int)VirtualKey::Q))
	{
		focus += (float)timer->GetElapsedSeconds();
		m_effect->SetFocus(focus);
	}
	if (input->KeyStatus((int)VirtualKey::A))
	{
		focus -= (float)timer->GetElapsedSeconds();
		m_effect->SetFocus(focus);
	}
	if (input->KeyStatus((int)VirtualKey::E))
	{
		diffuse += (float)timer->GetElapsedSeconds();
		m_effect->SetDiffuseConstant(diffuse);
	}
	if (input->KeyStatus((int)VirtualKey::D))
	{
		diffuse -= (float)timer->GetElapsedSeconds();
		m_effect->SetDiffuseConstant(diffuse);
	}
	if (input->KeyPressed((int)VirtualKey::Escape))
	{
		return MAIN_MENU;
	}
	CalculateLighting(input->GetMouseCoords());
	//Running represents that the GameState is still running
	return RUNNING;
}

void GameStates::MapEditor::Close()
{
	BrushManager::ClearEffect();
}

void GameStates::MapEditor::OnWindowResize()
{
	float width, height;
	auto graphics = EngineResources::GetGraphicsDevice();
	graphics->GetWindowDimensions(width, height);
	m_floor.SetDimensions(width, height);
}

void GameStates::MapEditor::Draw()
{
	auto graphics = EngineResources::GetGraphicsDevice();

	graphics->Draw(m_floor, true);

	for (auto& x : m_map)
		graphics->Draw(x);

	graphics->Draw(m_cursor, true);
}

void GameStates::MapEditor::UpdateCursor(bool buttonPressed, float elapsed)
{
	static float color = 0.0f;
	//Position calculation
	glm::vec2 mousePos = EngineResources::GetInputManager()->GetMouseCoords();
	//Color change
	static float addFactor = 0.0f;
	addFactor += elapsed;
	color = cosf(addFactor);

	float alpha = color;
	if (alpha < 0.7f)
		alpha = 0.7f;
	//m_cursor.SetColor(color, color, 1.0f, alpha);

	/*mousePos.y += m_cursor.GetScale().y;*/
	m_cursor.SetPosition(mousePos);
	//Check for mouse button click
	static bool bIsPressed = false;
	if (buttonPressed)
	{
		AddNewBlock(m_cursor);
	}
}

//Copy's the passed model and creates a new one from it
void GameStates::MapEditor::AddNewBlock(Graphics::Square model)
{
	Square newBlock;
	//newBlock.SetColor(model.GetColor());
	/*auto scale = model.GetScale();*/
	newBlock.SetDimensions(model.GetWidth(), model.GetHeight());
	newBlock.SetPosition(model.GetPosition());
	newBlock.SetBrush(model.GetBrush(), model.GetBrushType());

	//Adds to the new block vector described in the header file
	m_map.push_back(newBlock);
}

void GameStates::MapEditor::CalculateLighting(glm::vec2 pos)
{
	m_effect->SetFocusPosition(pos);
}
