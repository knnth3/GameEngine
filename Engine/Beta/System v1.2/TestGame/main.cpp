#include "Game.h"
#include "Engine_App.h"


int main()
{
	std::unique_ptr<System::App> app = std::make_unique<Engine_App>(800u, 600u);
	std::string name = "Test Game";
	int result = System::RunApp(name, app, System::flags::NONE);
	return result;
}