#include <Lime\Lime.h>
#include "Dungeon.h"

int main()
{
	std::unique_ptr<Lime::App> app = std::unique_ptr<Lime::App>(new Dungeon);
	int error = Lime::AppStarter::Start(L"Test Game", app);
	return error;
}