#include <Lime\App.h>
#include <chrono>
#include "GS_MainMenu.h"
//Dungeon is the main implementation of this Application
//It represents the high layer of this program and does things accorfing to
//-the gamestate it is on
//This class is already implemented and should not change very often
//All logic will be done in the gamestate




//Uses the namespace Lime since all functions in this class will be using the Lime engine
using namespace Lime;

//An inherited class from base (Lime::App) All functions nessesary can be found in the Lime::App header file
//-and are also shown here
class Dungeon :
	public Lime::App
{
public:
	Dungeon();
	~Dungeon();

	//state changes
	virtual void Tick();
	//****************Inherited Functions*************************
	virtual void OnInitialize() override;
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void OnSuspending() override;
	virtual void OnResuming() override;
	virtual void OnWindowMoved() override;
	virtual void OnWindowSizeChanged(int width, int height) override;
	//***************************************************************

private:
	//Helper functions
	void Update(float elapsed);
	void Render();
	void Clear();

	//Logic variables
	float camRot = 0.0f;
	int m_fps = 0;

	//Ptr to default camera
	std::shared_ptr<Camera> m_camera;
	//Ptr to current gameState
	std::unique_ptr<GameState> m_state;
	//used to calculate time elapsed
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

};