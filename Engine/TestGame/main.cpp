#include "Game.h"


int main()
{
	std::unique_ptr<System::App> app = std::make_unique<Game>();
	int result = System::RunApp(L"Test Game", app);
	return result;
}