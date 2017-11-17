#include "Biosphere.h"
#include <JSON\json.hpp>
#include <fstream>

using json = nlohmann::json;

PL::Biosphere::Biosphere(std::string Folder):
	m_folder(Folder)
{
	m_bClose = false;
	m_bSave = false;
}

PL_Actor_ID PL::Biosphere::SpawnActor(const std::string name)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	if (m_actors.size() < MAX_ACTORS_PER_SCENE)
	{
		m_actors.emplace_back(name);
		return (PL_Actor_ID)m_actors.size() - 1;
	}
	return ACTOR_ID_NONVALID;
}

bool PL::Biosphere::GetActor(const PL_Actor_ID id, PL_ActorData& data)
{
	std::lock_guard<std::mutex> lock(m_threadLock);
	if (m_actors.size() > id && id >= 0)
	{
		//Populate data with actor stats
		data.Name = m_actors[id].GetName();
		data.MaxHealth = m_actors[id].GetMaxHealth();
		data.CurrentHealth = m_actors[id].GetCurrentHealth();
		return true;
	}
	return false;
}

void PL::Biosphere::Update()
{
	while (!m_bClose)
	{
		if (m_bSave)
		{
			json file;
			PL_ActorData data;
			if (GetActor(0, data))
			{
				file["Name"] = data.Name;
				file["Max Health"] = data.MaxHealth;
				file["Current Health"] = data.CurrentHealth;
				std::string path = m_folder + "\\pretty.json";
				std::ofstream o(path);
				o << std::setw(4) << file << std::endl;
				o.close();
			}
			m_bSave = false;
		}
	}
}

void PL::Biosphere::Close()
{
	m_bClose = true;
}

void PL::Biosphere::WriteToDisk()
{
	m_bSave = true;
}
