#include "Populace.h"
#include <future>
#include <Windows.h>


bool PL_Initialize(std::string directory)
{
	size_t pos = directory.find_last_of('\\');
	std::string dir(directory.begin(), directory.begin() + pos);
	std::string folderName = dir + "\\Populace";
	if (CreateDirectory(PL::ToWstring(folderName).c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		PL::m_biosphere = new PL::Biosphere(folderName);
		PL::m_thread = std::thread(PL::Update_Biosphere);
		return true;
	}
	return false;
}

void PL_Close()
{
	PL::m_biosphere->Close();
	PL::m_thread.join();
	delete PL::m_biosphere;
}

PL_Actor_ID PL_CreateActor(std::string name)
{
	return PL::m_biosphere->SpawnActor(name);
}

bool PL_GetActorData(const PL_Actor_ID id, PL::PL_ActorData & data)
{
	return PL::m_biosphere->GetActor(id, data);
}

void PL_WriteToDisk()
{
	PL::m_biosphere->WriteToDisk();
}

void PL::Update_Biosphere()
{
	m_biosphere->Update();
}
