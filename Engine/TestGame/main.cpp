#include <Lime\DX11WindowApp.h>
#include <Lime\DX11Renderer.h>
#include <chrono>
#include "Game.h"
#include <iterator>

using namespace Lime;
int main()
{
	int error = 0;
	DX11WindowApp app(L"Test Game", std::make_unique<Game>());
	error = app.Initialize();
	if (!error)
		app.Run();

	return error;
}