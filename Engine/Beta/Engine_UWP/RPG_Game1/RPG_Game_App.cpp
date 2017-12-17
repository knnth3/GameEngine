#include "pch.h"
#include "RPG_Game_App.h"
#include <Graphics\TextureLoader.h>
#include <Graphics\MeshLoader.h>
#include <Lime_Engine\EngineResources.h>
#include "Core\GS_MainMenu.h"
#include "Core\GS_MapEditor.h"
#include "Core\GS_SceneManager.h"
#include "GS_TestingRealm.h"

using namespace LIME_ENGINE;
using namespace Windows::System;
using namespace Graphics;
using namespace GameStates;


RPG_Game_App::RPG_Game_App()
{
	SetClearColor(0.2f, 0.2f, 0.2f);
	SetDefaultDimensions(832.0f, 640.0f);
	m_currentState = MAIN_MENU;
}

void RPG_Game_App::OnUpdate()
{
	auto result = m_state->Update();
	SwitchStates(result);
}

void RPG_Game_App::OnRender()
{
	m_state->Draw();
}

void RPG_Game_App::OnResume()
{
	SwitchStates(m_currentState);
	EngineResources::GetConsole()->SetToggleButton(192);
}

void RPG_Game_App::OnSuspend()
{
	m_state->Close();
	m_state.release();
}

void RPG_Game_App::OnWindowSizeChanged()
{
	m_state->OnWindowResize();
}

void RPG_Game_App::SwitchStates(GameStates::States state)
{
	switch (state)
	{
	case GameStates::MAIN_MENU:
	{
		std::unique_ptr<GameState> newState = std::unique_ptr<MainMenu>(new MainMenu());
		if (newState->CreateDeviceIndependentResources())
		{
			InitializeNewState(newState);
			m_currentState = MAIN_MENU;
		}
	}
	break;
	case GameStates::MAP_EDITOR:
	{
		std::unique_ptr<GameState> newState = std::unique_ptr<MapEditor>(new MapEditor());
		if (newState->CreateDeviceIndependentResources())
		{
			InitializeNewState(newState);
			m_currentState = MAP_EDITOR;
		}
	}
	break;
	case GameStates::SCENE_MANAGER:
	{
		std::unique_ptr<GameState> newState = std::unique_ptr<SceneManager>(new SceneManager());
		if (newState->CreateDeviceIndependentResources())
		{
			InitializeNewState(newState);
			m_currentState = SCENE_MANAGER;
		}
	}
	break;
	case GameStates::TESTING_REALM:
	{
		std::unique_ptr<GameState> newState = std::unique_ptr<TestingRealm>(new TestingRealm());
		if (newState->CreateDeviceIndependentResources())
		{
			InitializeNewState(newState);
			m_currentState = TESTING_REALM;
		}
	}
	break;
	case GameStates::RUNNING:
	default:
		break;
	}
}

void RPG_Game_App::InitializeNewState(std::unique_ptr<GameState>& newState)
{
	if (m_state)
		m_state->Close();

	m_state.swap(newState);
	m_state->CreateDeviceDependentResources();
}
