#include "Populace.h"
#include <future>


bool PL_Initialize(const std::string DomainName)
{
	//Get the path that the exe is running in
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	int bytes = GetModuleFileName(hModule, path, MAX_PATH);
	if (bytes == 0)
		return false;

	std::string directory = PL::ToString(path);
	size_t pos = directory.find_last_of('\\');
	std::string dir(directory.begin(), directory.begin() + pos);
	std::string folderName = dir + "\\Populace\\";
	if (!PL::CreateDir(folderName))
		return false;

	PL::m_biosphere = new PL::Biosphere(folderName, DomainName);
	PL::m_thread = std::thread(PL::Update_Biosphere);
	return true;
}

void PL_Close()
{
	PL::m_biosphere->Close();
	while (!PL::m_thread.joinable())
	{

	}
	PL::m_thread.join();
	delete PL::m_biosphere;
}

bool PL_CreateActor(std::string name)
{
	return PL::m_biosphere->SpawnActor(name);
}

bool PL_KillActor(std::string name)
{
	return PL::m_biosphere->KillActor(name);
}

bool PL_GetActorData(const std::string name, PL_ActorData & data)
{
	return PL::m_biosphere->GetActor(name, data);
}

inline void PL_ClearDeadActors()
{
	PL::m_biosphere->ClearDeadActors();
}

inline bool PL_GiveItem(const std::string name, const PL_Item_Desc item)
{
	return PL::m_biosphere->GiveItem(name, item);
}

void PL_Save()
{
	PL::m_biosphere->Save();
}

void PL::Update_Biosphere()
{
	m_biosphere->Update();
}
