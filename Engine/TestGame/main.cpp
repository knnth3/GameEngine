#include <Lime\Lime.h>
#include <chrono>
#include "Game.h"
#include <iterator>

using namespace Lime;
int main()
{
	std::unique_ptr<App> app = std::unique_ptr<App>(new Game);
	int error = AppStarter::Start(L"Test Game", app);
	return error;
}