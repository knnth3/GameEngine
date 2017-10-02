#pragma once
#include <Lime\DX11App.h>
#include <Lime\InputManager.h>
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
	std::shared_ptr<Lime::Camera> m_camera;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	std::shared_ptr<Model3D> model;
	std::shared_ptr<Model3D> model2;
	std::shared_ptr<Lime::TextController> controller;
	float camRot = 0.0f;
	float rot = 0.0f;
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colormodr = 1;
	int colormodg = 1;
	int colormodb = 1;
};

