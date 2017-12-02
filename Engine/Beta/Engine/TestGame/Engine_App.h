#pragma once
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <Lime_Engine\EngineApp.h>
#include <Lime_Engine\Cube.h>
#include <Lime_Engine\Lime_String.h>
#include <chrono>
#include "Populace\Populace.h"

//Objects desired to be drawn to screen follow a specific pattern:
//Make sure to call these functions in the appropriate app functions.

//Creation stage:
//	Shapes::Cube c; or Shapes::Square s;
//Initialization stage:
//	c.SetColor(r,g,b); etc
//Update stage:
//	c.SetPosition(x,y,z); etc
//Draw stage:
//	Order matters (painter's algorithm)
//	graphics->Draw(c);


using namespace LIME_ENGINE;
class Engine_App:
	public EngineApp
{
public:
	Engine_App(uint16_t windowWidth, uint16_t windowHeight, std::string programPath);

	//Overloaded functions
	virtual void OnStartUp() override;
	virtual void OnShutdown() override;
	virtual void OnTick() override;

private:
	void Update(float elapsed);
	void Render();

	//used to calculate time elapsed
	int m_fps = 0;
	float movespeed;
	bool stress;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	Lime_String m_fpsStr;

	//Your objects here
	std::shared_ptr<Shapes::Cube> m_cube;
};

