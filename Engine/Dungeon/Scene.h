#pragma once
//Scene Class is the uper level of the game.
//All "Hard-Code logic" goes in an inherted class from this.
//EX: If making a new level, Create a class and inherit this. Your code goes in the new level class.
//PLease encapulate your new scenes in the Scenes namespace
//
//Some functions provided below are not required to override but can be useful.
//EX: GetNewScene() dictates what scene will be implemented if the RunState calls for a new scene.
//Null is default so the call for a new scene will be ingored.
//
//To do special operations like the one above, you will have to 
//change the Runstate manually with SetCurrentState().
//A list of available Runstates is provided below
//EX: if SetCurrentState() is called with the perameter PAUSE, the current state will suspend
// and then the pause scene will become active (if one is given otherwise nothing will happen).
//
//File loading Example
//
//#include "DungeonMapTileset.h"
//#include <Lime\ModelLoader.h>
//using namespace Lime::Model;
//using namespace std;
//void LoadFile(shared_ptr<Lime::RenderBatch>& rend, shared_ptr<Lime::Camera>& camera)
//{
//	DMT inFile;
//	vector<shared_ptr<Model3D>> m_map;
//	FileManager::LoadFile("Maps/Level1.dmt", inFile);
//
//	for (auto x : inFile.objects)
//	{
//		m_map.emplace_back(make_shared<Model3D>(MeshLoader::LoadModel(x.vertices, x.indices)));
//		auto & model = m_map.at(m_map.size() - 1);
//		model->SetPosition(x.position);
//		model->Scale(x.scale);
//		rend->Add3DModel(model);
//	}
//	if (m_map.size() > 0)
//	{
//		camera->AttachToModel(m_map[0]);
//	}
//
//}



#include "GameState.h"
#include <Lime\RenderBatch.h>


enum class RunState
{
	INITIALIZE = 0,
	UPDATE,
	PAUSE,
	RESUME,
	CLOSE,
	NEW_SCENE,
	MAIN_MENU,
};


class Scene
{
public:
	Scene(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera);
	~Scene();

	void SetWindowDimensions(const int windowWidth, const int windowHeight);
	const RunState GetCurrentSate();
	void SetCurrentState(RunState state);
	void Init();

	//Overload functions
	virtual void Initialize() = 0;
	virtual void Update(float time, std::shared_ptr<Lime::InputManager>& input) = 0;

	//Overloadable functions
	virtual std::unique_ptr<Scene> GetNewScene();
	virtual std::unique_ptr<Scene> GetLoadingScene();


protected:
	//Functions only Scenes can run
	const uint32_t GetWindowWidth();
	const uint32_t GetWindowHeight();
	std::shared_ptr<Lime::RenderBatch> GetRender();
	std::shared_ptr<Lime::Camera> GetCamera();

private:
	std::shared_ptr<Lime::RenderBatch> m_render;
	std::shared_ptr<Lime::Camera> m_camera;
	RunState m_currentState;
	uint32_t m_windowWidth;
	uint32_t m_windowHeight;
};

