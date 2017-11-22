#include "Biosphere.h"
#include <fstream>

using json = nlohmann::json;

PL::Biosphere::Biosphere(std::string Folder):
	m_folder(Folder),
	m_filename("\\PL_DAT.json")
{
	m_bClose = false;
	m_bSave = false;
	m_bClearDead = false;
}

void PL::Biosphere::SpawnActor(const std::string name)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	m_actors.emplace(name, name);
}

bool PL::Biosphere::GetActor(const std::string name, PL_ActorData& data)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	auto actor = m_actors.find(name);
	if (actor != m_actors.end())
	{
		//Populate data with actor stats
		data = actor->second;
		return true;
	}
	return false;
}

void PL::Biosphere::GetAllActors(std::vector<PL_ActorData>& data)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	for (auto actor : m_actors)
		data.emplace_back(actor.second);
}

void PL::Biosphere::Update()
{
	while (!m_bClose)
	{
		ClearDead();
		Save();
	}
}

void PL::Biosphere::Close()
{
	m_bClose = true;
}

bool PL::Biosphere::KillActor(const std::string name)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	auto actor = m_actors.find(name);
	if (actor != m_actors.end())
	{
		actor->second.SetLivingStatus(false);
		return true;
	}

	return false;
}

void PL::Biosphere::ClearDeadActors()
{
	m_bClearDead = true;
}

bool PL::Biosphere::GiveItem(const std::string name, const PL_Item item)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	auto actor = m_actors.find(name);
	if (actor != m_actors.end())
	{
		actor->second.AddItem(item);
		return true;
	}
	return false;
}

void PL::Biosphere::WriteToDisk()
{
	m_bSave = true;
}

void PL::Biosphere::ClearDead()
{
	if (m_bClearDead)
	{
		std::vector<std::string> m_deadActors;
		for (auto actor : m_actors)
			if (actor.second.IsAlive())
				m_deadActors.push_back(actor.first);

		for (auto name : m_deadActors)
			m_actors.erase(name);

		m_bClearDead = false;
	}
}

void PL::Biosphere::Save()
{
	if (m_bSave)
	{
		json file;
		std::string path = m_folder + m_filename;
		std::vector<PL_ActorData> data;
		GetAllActors(data);
		file = json{ "Actors", data };
		std::ofstream o(path);
		o << std::setw(4) << file << std::endl;
		o.close();
		m_bSave = false;
	}
}