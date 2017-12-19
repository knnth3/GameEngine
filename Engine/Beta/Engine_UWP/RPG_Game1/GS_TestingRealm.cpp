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
	m_player.Scale(100.0f, 100.0f, 100.0f);
	m_player.SetPosition(0.0f, 200.0f, 0.0f);
	return true;
}

void GameStates::TestingRealm::CreateDeviceDependentResources()
{
	std::string character = "Assets/textures/grid_tile.png";
	TextureID tex = TextureLoader::CreateNewTexture(character, character, character, character, character, character);
	MeshID mesh = MeshLoader::CreatePlane(100.0f, 100.0f, 10.0f, 10.0f);

	m_floor.SetMesh(mesh);
	m_floor.SetTexture(tex);
	EngineResources::GetGraphicsDevice()->GetCamera()->AttachToModel(m_floor);
	EngineResources::GetGraphicsDevice()->GetCamera()->EnforceBounds(false);
}

GameStates::States GameStates::TestingRealm::Update()
{
	static float rotation = 0;
	static bool wireframe = false;
	auto input = EngineResources::GetInputManager();
	auto camera = EngineResources::GetGraphicsDevice()->GetCamera();
	auto timer = EngineResources::GetTimer();
	rotation  = 2.3 * timer->GetElapsedSeconds();
	if (input->KeyPressed((int)VirtualKey::Escape))
	{
		return MAIN_MENU;
	}
	if (input->KeyPressed((int)VirtualKey::L))
	{
		wireframe = !wireframe;
		EngineResources::GetGraphicsDevice()->Wireframe(wireframe);
	}
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
	if (input->KeyStatus((int)VirtualKey::Q))
	{
		camera->Zoom(-rotation * 100.0f);
	}
	if (input->KeyStatus((int)VirtualKey::E))
	{
		camera->Zoom(rotation * 100.0f);
	}
	return RUNNING;
}

void GameStates::TestingRealm::Draw()
{
	EngineResources::GetGraphicsDevice()->Draw(m_floor);
	EngineResources::GetGraphicsDevice()->Draw(m_player);
}

void GameStates::TestingRealm::Close()
{
	TextureLoader::Clear();
	MeshLoader::Clear();
	EngineResources::GetGraphicsDevice()->Reset();
	EngineResources::GetGraphicsDevice()->GetCamera()->EnforceBounds(true);
	EngineResources::GetGraphicsDevice()->Wireframe(false);
}

void GameStates::TestingRealm::OnWindowResize()
{
}
