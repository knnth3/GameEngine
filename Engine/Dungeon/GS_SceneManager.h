#pragma once
#include "GameState.h"
#include "Scene.h"
#include <Lime\RenderBatch.h>

//Handles all Scene switch/run logic
class SceneManager:
	public GameState
{
public:
	SceneManager(std::unique_ptr<Scene>& beginScene);
	~SceneManager();

	virtual GameStates::States Update(float time, std::shared_ptr<Lime::InputManager>& input);
	virtual void Initialize(const int windowWidth, const int windowHeight);
	virtual void Close();
	virtual void OnWindowResize(int width, int height);

private:
	uint32_t m_windowWidth;
	uint32_t m_windowHeight;
	std::shared_ptr<Character> m_character;
	std::unique_ptr<Scene> m_onSuspend;
	std::unique_ptr<Scene> m_active;

};

