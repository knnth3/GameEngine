#include "Engine_App.h"

int main(int argc, char **argv)
{
	std::string EXE_PATH;
	if (argc > 0)
		EXE_PATH = argv[0];
	if (EXE_PATH.empty())
		return -1;

	std::unique_ptr<System::App> app = std::make_unique<Engine_App>(800u, 600u, EXE_PATH);
	std::string name = "Test Game";
	int result = System::RunApp(name, app, System::flags::NONE);
	return result;
}