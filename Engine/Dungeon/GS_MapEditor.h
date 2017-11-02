#pragma once
#include "GameState.h"
#include <Lime\DX11Graphics.h>

//Game state for Level editor
//Uses pre made models to tile map
//The character model is not my own. By 3dregenerator.
//Downloaded from https://free3d.com/download-page.php?url=puo-3971-61889
//****!All code implementations will be done in the cpp file!*****



namespace GameStates
{
	class MapEditor :
		public GameState
	{
	public:
		//This constructor takes in the render batch and the active camera
		MapEditor(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera);

		/////////////Inherited functions from Gamesate/////////////////////////
		//--If creating a new function make sure to write *override* at the end of the function implementation
		//--This does nothing but it does make it clear that it's inherited
		//--The same applies to the virtual before the function

		//Function called every frame
		virtual States Update(float time, std::shared_ptr<Lime::InputManager>& input) override;

		//State Changes
		virtual void Initialize(int windowWidth, int windowHeight) override;
		virtual void Close() override;
		virtual void OnWindowResize(int width, int height) override;

		/////////////////////////////////////////////////////////////////////////

		//Funtion implementation to draw every object
		//It is separated from update for organization purposes
		void Draw();

	private:
		//All these private functions cannot be accessed by objects that create an instance this class
		//These functions are specifically for this class to use to do its operations
		//In any case, they can be called helper functions

		//Creates a floor in the scene with a desired length and width
		void CreateFloor(int length, int width, MeshID id);
		//All mouse logic per frame goes in here including the way the cursor changes color and the cell snapping
		void UpdateCursor(float time, std::shared_ptr<Lime::InputManager>& input);
		//Spawn a new block
		void AddNewBlock(std::shared_ptr<Lime::Model::Model3D>& model);

		//All variables that will be relevant to the object goes at the bottom of the class header file
		//-Do not forget to initialze each variable
		//-Sometimes it's not nessesary but for most cases zero-ing the object doesnt hurt
		float camRot = 0.0f;

		//The model that the user is using
		std::shared_ptr<Lime::Model::Model3D> m_user;

		//The cursor that follows the mouse in the game
		std::shared_ptr<Lime::Model::Model3D> m_cursor;

		//Currently Active camera (User)
		std::shared_ptr<Lime::Camera> m_camera;

		//All tiles go in here
		std::vector<std::shared_ptr<Lime::Model::Model3D>> m_map;

		//The render batch is what is used to print objects to the screen
		std::shared_ptr<Lime::RenderBatch> m_render;
	};

}
