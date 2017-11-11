#include "Game.h"


int main()
{
	std::unique_ptr<System::App> app = std::make_unique<Game>(800u, 600u);
	std::string name = "Test Game";
	int result = System::RunApp(name, app);
	return result;
}