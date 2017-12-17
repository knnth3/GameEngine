#include "pch.h"
#include "GS_TestingRealm.h"
#include "Lime_Engine\EngineResources.h"
#include <Graphics\MeshLoader.h>
#include <Graphics\TextureLoader.h>

using namespace Graphics;
using namespace LIME_ENGINE;
using namespace Windows::System;

bool GameStates::TestingRealm::CreateDeviceIndependentResources()
{
	m_floor.Scale(1000.0f, 1.0f, 1000.0f);
	m_player.Scale(100.0f, 100.0f, 1.0f);
	m_playerBack.Scale(100.0f, 100.0f, 1.0f);

	m_player.Rotate(0.0f, 0.0f, (float)M_PI);
	m_playerBack.Rotate(0.0f, (float)M_PI, (float)M_PI);
	m_player.SetPosition(0.0f, 200.0f, 0.0f);
	m_playerBack.SetPosition(0.0f, 200.0f, 0.0f);
	return true;
}

void GameStates::TestingRealm::CreateDeviceDependentResources()
{
	std::string character = "Assets/textures/Basic_Character/rpg_sprite_walk.png";
	TextureID tex = TextureLoader::CreateNewTexture(character, character, character, character, character, character);
	MeshID mesh = MeshLoader::LoadModel("Assets/Models/game_piece.obj");
	m_player.SetMesh(mesh);
	m_player.SetTexture(tex);
	m_playerBack.SetMesh(mesh);
	m_playerBack.SetTexture(tex);
	EngineResources::GetGraphicsDevice()->GetCamera()->AttachToModel(m_floor);
}

GameStates::States GameStates::TestingRealm::Update()
{
	static float rotation = 0;
	auto input = EngineResources::GetInputManager();
	auto camera = EngineResources::GetGraphicsDevice()->GetCamera();
	auto timer = EngineResources::GetTimer();
	rotation  = 2.3 * timer->GetElapsedSeconds();
	if(input->KeyStatus((int)VirtualKey::W))
	{
		camera->Rotate(rotation, 0.0f, 0.0f);
	}
	if (input->KeyStatus((int)VirtualKey::A))
	{
		camera->Rotate(0.0f, rotation, 0.0f);
	}
	if (input->KeyStatus((int)VirtualKey::S))
	{
		camera->Rotate(-rotation, 0.0f, 0.0f);
	}
	if (input->KeyStatus((int)VirtualKey::D))
	{
		camera->Rotate(0.0f, -rotation, 0.0f);
	}
	return RUNNING;
}

void GameStates::TestingRealm::Draw()
{
	EngineResources::GetGraphicsDevice()->Draw(m_floor);
	EngineResources::GetGraphicsDevice()->Draw(m_player);
	EngineResources::GetGraphicsDevice()->Draw(m_playerBack);
}

void GameStates::TestingRealm::Close()
{
	EngineResources::GetGraphicsDevice()->Reset();
}

void GameStates::TestingRealm::OnWindowResize()
{
}
