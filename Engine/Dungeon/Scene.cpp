#include "Scene.h"



Scene::Scene(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera)
{
	m_currentState = RunState::INITIALIZE;
	m_render = rend;
	m_camera = camera;
}

Scene::~Scene()
{
}

void Scene::SetWindowDimensions(const int windowWidth, const int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

const RunState Scene::GetCurrentSate()
{
	return m_currentState;
}

void Scene::Init()
{
	Initialize();
	m_currentState = RunState::UPDATE;
}

std::unique_ptr<Scene> Scene::GetNewScene()
{
	return nullptr;
}

std::unique_ptr<Scene> Scene::GetLoadingScene()
{
	return nullptr;
}

const uint32_t Scene::GetWindowWidth()
{
	return m_windowWidth;
}

const uint32_t Scene::GetWindowHeight()
{
	return m_windowHeight;
}

std::shared_ptr<Lime::RenderBatch> Scene::GetRender()
{
	return m_render;
}

std::shared_ptr<Lime::Camera> Scene::GetCamera()
{
	return m_camera;
}

void Scene::SetCurrentState(RunState state)
{
	m_currentState = state;
}
