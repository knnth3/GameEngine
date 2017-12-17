#pragma once
#include <Lime_Engine\InputManager.h>
#include <Graphics\Graphics.h>

//The gamestate class is the basic foundation for any and all states.
//All the logic will go into an inherited class of this
//EX: MAP EDITOR, MAIN MENU
//For organization purposes, use GS_ naming convention for any gamestates


namespace GameStates
{
	//New states will be added here to be able to
	//-call them for their creation
	enum States
	{
		RUNNING = 0,
		MAIN_MENU,
		MAP_EDITOR,
		SCENE_MANAGER,
		TESTING_REALM
	};
}

class GameState
{
public:
	//Function called every frame
	//Looks for GameStates::States::(state to change to) on return
	//All gamestates will require the (elapsed time for the frame) and (controller inmput) function.
	virtual GameStates::States Update() = 0;
	virtual void Draw() = 0;
	virtual bool CreateDeviceIndependentResources() = 0;
	virtual void CreateDeviceDependentResources() = 0;
	virtual void Close() = 0;
	virtual void OnWindowResize() = 0;
};

