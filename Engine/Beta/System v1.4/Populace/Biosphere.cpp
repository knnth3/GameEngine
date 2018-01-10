#include "Biosphere.h"
#include "PL_Library.h"
#include <fstream>

using json = nlohmann::json;

PL::Biosphere::Biosphere(const std::string Folder, const std::string domain):
	m_folder(Folder)
{
	PL_Library::Initialize(m_folder, domain);
	m_bClose = false;
	m_bSave = false;
	m_bClearDead = false;
}

bool PL::Biosphere::SpawnActor(const std::string name)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	return PL_Library::CreateActor(name);
}

bool PL::Biosphere::GetActor(const std::string name, PL_ActorData& data)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	return PL_Library::GetActor(name, data);
}

void PL::Biosphere::Update()
{
	while (!m_bClose)
	{
		ClearDead();
		WriteToDisk();
	}
}

void PL::Biosphere::Close()
{
	m_bClose = true;
}

bool PL::Biosphere::KillActor(const std::string name)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	return PL_Library::KillActor(name);
}

void PL::Biosphere::ClearDeadActors()
{
	m_bClearDead = true;
}

bool PL::Biosphere::GiveItem(const std::string name, const PL_Item_Desc item)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	if(item.Type.compare("Apparel") == 0)
		return PL_Library::GiveApparel(name, item.Name);
	else if (item.Type.compare("Weapon") == 0)
		return PL_Library::GiveWeapon(name, item.Name);

	return false;
}

void PL::Biosphere::Save()
{
	m_bSave = true;
}

void PL::Biosphere::ClearDead()
{
	if (m_bClearDead)
	{
		PL_Library::ClearDeadActors();
		m_bClearDead = false;
	}
}

void PL::Biosphere::WriteToDisk()
{
	if (m_bSave)
	{
		PL_Library::Save();
		m_bSave = false;
	}
}