#include "GS_SceneManager.h"



SceneManager::SceneManager(std::unique_ptr<Scene>& beginScene)
{
	m_onSuspend = nullptr;
	m_active = nullptr;
	m_active.swap(beginScene);
	Character_Info info;
	info.player_name = "Developer";
	info.character_name = "Alfie";
	m_character = std::make_shared<Character>(info);
}

SceneManager::~SceneManager()
{
}

GameStates::States SceneManager::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	GameStates::States result = GameStates::States::RUNNING;
	if (m_active)
	{
		switch (m_active->GetCurrentSate())
		{
		case RunState::PAUSE:
		case RunState::RESUME:
			m_active->SetCurrentState(RunState::UPDATE);
			if (m_onSuspend)
				m_active.swap(m_onSuspend);
			break;

		case RunState::CLOSE:
			result = GameStates::States::CLOSE;

		case RunState::UPDATE:
			m_active->Update(time, input);
			break;

		case RunState::INITIALIZE:
			input->Reset();
			m_active->Init(m_character);
			m_active->SetWindowDimensions(m_windowWidth, m_windowHeight);
			m_onSuspend.swap(m_active->GetLoadingScene());
			if (m_onSuspend)
			{
				m_onSuspend->Init(m_character);
				m_active->SetWindowDimensions(m_windowWidth, m_windowHeight);
			}
			break;

		case RunState::NEW_SCENE:
			m_active->SetCurrentState(RunState::UPDATE);
		{
			auto newScene = m_active->GetNewScene();
			if (newScene)
				m_active.swap(newScene);
		}
			break;

		case RunState::MAIN_MENU:
			result = GameStates::States::MAIN_MENU;
			break;

		default:
			break;
		}
	}
	return result;
}

void SceneManager::Initialize(const int windowWidth, const int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

void SceneManager::Close()
{
}

void SceneManager::OnWindowResize(int width, int height)
{
	Initialize(width, height);
	if (m_active)
		m_active->SetWindowDimensions(width, height);
}
