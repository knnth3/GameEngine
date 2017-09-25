#pragma once
#include <Lime\DX11App.h>
#include <chrono>

using namespace Lime;
class Game : public DX11App
{
public:
	Game();

	virtual void Initialize();
	virtual void Tick();

	//state changes
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();
	virtual void OnWindowMoved();
	virtual void OnWindowSizeChanged(int width, int height);

private:
	void Update(float elapsed);
	void Render();
	void Clear();

	//Testing
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	std::shared_ptr<Model2> model;
	float rot = 0.0f;
};

