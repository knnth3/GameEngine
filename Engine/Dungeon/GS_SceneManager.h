#pragma once
#include "GameState.h"
#include "Scene.h"
#include <Lime\RenderBatch.h>

//Handles all Scene switch/run logic
class SceneManager:
	public GameState
{
public:
	SceneManager(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera);
	SceneManager(std::unique_ptr<Scene>& loadingScene, std::unique_ptr<Scene>& beginScene);
	~SceneManager();

	virtual GameStates::States Update(float time, std::shared_ptr<Lime::InputManager>& input);
	virtual void Initialize(int windowWidth, int windowHeight);
	virtual void Close();
	virtual void OnWindowResize(int width, int height);

private:
	std::unique_ptr<Scene> m_onSuspend;
	std::unique_ptr<Scene> m_active;

	//All tiles go in here
	std::vector<std::shared_ptr<Lime::Model::Model3D>> m_map;

};

