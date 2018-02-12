#pragma once
#include <Lime_Engine\EngineApp.h>
#include "Core\GameState.h"


class RPG_Game_App:
	public LIME_ENGINE::EngineApp
{
public:
	RPG_Game_App();

	//Required
	virtual void OnUpdate()override;
	virtual void OnRender()override;
	virtual void OnResume()override;
	virtual void OnSuspend()override;
	virtual void OnWindowSizeChanged()override;

private:
	void SwitchStates(GameStates::States state);
	void InitializeNewState(std::unique_ptr<GameState>& newState);

	GameStates::States m_currentState;
	std::unique_ptr<GameState> m_state;
};

