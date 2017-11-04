#pragma once
#include <System\Window.h>

class Game:
	public System::App
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
};

