#pragma once
#include "Shared.h"
#include <Lime\InputManager.h>

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
		CLOSE = -1,
		RUNNING = 0,
		MAIN_MENU= 1,
		MAP_EDITOR = 2
	};
}

class GameState
{
public:
	//This doesn't require the render batch as not every gamestate requires a visual representation
	//(= default) just means there wont be any special constructor for this class and the compiler will instead do nothing for this function
	//-Alternatively you can just delete it but I do it for organization or if I ever want to write one
	GameState() = default;

	//Function called every frame
	//Looks for GameStates::States::(state to change to) on return
	//All gamestates will require the (elapsed time for the frame) and (controller inmput) function.
	virtual GameStates::States Update(float time, std::shared_ptr<Lime::InputManager>& input) = 0;

	//State Changes
	//All of the code that has heavy calculations should be done in this function before the GS loads
	virtual void Initialize(int windowWidth, int windowHeight) = 0;

	//All the code that requires you to unalocate, clear or release is done here
	virtual void Close() = 0;

	//This function is used to update objects that require the window specs
	//!Notice! this functon does not have an ( = 0 ) at the end
	//-This means that the function does not need to be in the class that inherits this class
	//-The reasoning was the same for the Gamestate() initializer
	//-If not inherited, this function will do nothing
	virtual void OnWindowResize(int width, int height);
};

