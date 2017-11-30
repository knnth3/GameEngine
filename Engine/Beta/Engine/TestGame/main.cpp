#include "Engine_App.h"
#include "Populace\Populace.h"



using namespace std;
int main(int argc, char **argv)
{
	//Grabs the directory
	std::string EXE_PATH;
	if (argc > 0)
		EXE_PATH = argv[0];
	if (EXE_PATH.empty())
		return -1;

	//Tests Graphical app
	//std::unique_ptr<System::App> app = std::make_unique<Engine_App>(800u, 600u, EXE_PATH);
	//std::string name = "Test Game";
	//int result = System::RunApp(name, app, System::flags::NONE);
	//return result;

	//Tests Populace API (Everything has PL_ as prefix)
	PL_Initialize("Asgaurd");

	//Create Actors
	if (!PL_CreateActor("Eric"))
		cout << "-Actor Eric  already exists." << endl;
	if(!PL_CreateActor("Alfie"))
		cout << "-Actor Alfie already exists." << endl;

	PL_Item_Desc desc;
	desc.Name = "Wooden Longsword";
	desc.Type = "Weapon";
	if(!PL_GiveItem("Eric", desc))
		cout << "-Wooden Longsword is not a valid weapon." << endl;

	//Poll Actor Data for usage
	PL_ActorData data;
	bool result =  PL_GetActorData("Eric", data);
	if (result)
	{
		cout << "Name:           " << data.Name << endl;
		cout << "Current Health: " << data.CurrentHealth << endl;
		cout << "Alive:          " << data.IsAlive << endl;
	}
	PL_Save();
	PL_Close();
	system("pause");
	return 0;
}