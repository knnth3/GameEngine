#pragma once
#include <Lime\App.h>
class Game2:
	public Lime::App
{
public:
	Game2();

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
	std::shared_ptr<Lime::Camera> m_camera;
	std::shared_ptr<Lime::Model::Model3D> m_character;
};

