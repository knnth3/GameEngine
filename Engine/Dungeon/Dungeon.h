#include <Lime\App.h>
#include <chrono>
#include "GS_MainMenu.h"
using namespace Lime;

class Dungeon :
	public Lime::App
{
public:
	Dungeon();
	~Dungeon();

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

	//Logic
	float camRot = 0.0f;
	int m_fps = 0;
	std::shared_ptr<Camera> m_camera;
	std::unique_ptr<GameState> m_state;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

};