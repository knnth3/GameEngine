#pragma once
#include <Lime\App.h>
#include <Lime\InputManager.h>
#include <chrono>

using namespace Lime;
class Game : public App
{
public:
	Game();

	//state changes
	virtual void Tick();
	virtual void OnInitialize() override;
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void OnSuspending() override;
	virtual void OnResuming() override;
	virtual void OnWindowMoved() override;
	virtual void OnWindowSizeChanged(int width, int height) override;

private:
	void Update(float elapsed);
	void Render();
	void Clear();

	//Testing
	std::shared_ptr<Camera> m_camera;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colormodr = 1;
	int colormodg = 1;
	int colormodb = 1;
};

