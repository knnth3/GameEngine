#include "PL_Library.h"
#include <fstream>
#include <iostream>

std::map<std::string, PL::PL_Apparel> PL::PL_Library::m_apparel;
std::map<std::string, PL::PL_Weapon> PL::PL_Library::m_weapons;
std::map<std::string, PL::PL_Apparel> PL::PL_Library::m_apparel_ext;
std::map<std::string, PL::PL_Weapon> PL::PL_Library::m_weapons_ext;
std::map<std::string, PL::PL_Actor> PL::PL_Library::m_actors;
std::map<std::string, PL::PL_Transport> PL::PL_Library::m_transportations;
std::string PL::PL_Library::m_folder;
std::string PL::PL_Library::m_domain;

bool PL::PL_Library::Initialize(const std::string folder, const std::string domain)
{
	m_domain = domain;
	m_folder = folder;

	//Load save data
	json j;
	if (LoadJSON(j, m_folder + m_domain + PL_DATA_EXT))
		ProcessJSON(j);

	//Load add-ons
	LoadAddons();
	return true;
}

bool PL::PL_Library::GiveMount(const std::string actor, const std::string item)
{
	auto found = m_transportations.find(item);
	if (found != m_transportations.end())
	{
		m_actors[actor].AddMount(m_transportations.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::GiveApparel(const std::string actor, const std::string item)
{
	//Search through both add-ons and base items
	auto base = m_apparel.find(item);
	auto extended = m_apparel_ext.find(item);
	if (base != m_apparel.end())
	{
		m_actors[actor].AddItem(m_apparel.at(item));
		return true;
	}
	else if (extended != m_apparel_ext.end())
	{
		m_actors[actor].AddItem(m_apparel_ext.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::GiveWeapon(const std::string actor, const std::string item)
{
	//Search through both add-ons and base items
	auto base = m_weapons.find(item);
	auto extended = m_weapons_ext.find(item);
	if (base != m_weapons.end())
	{
		m_actors[actor].AddItem(m_weapons.at(item));
		return true;
	}
	else if (extended != m_weapons_ext.end())
	{
		m_actors[actor].AddItem(m_weapons_ext.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::CreateActor(const std::string name)
{
	auto found = m_actors.find(name);
	if (found == m_actors.end())
	{
		m_actors[name] = PL_Actor(name);
		return true;
	}
	return false;
}

bool PL::PL_Library::CreateActor(const PL_ActorData data)
{
	auto found = m_actors.find(data.Name);
	if (found == m_actors.end())
	{
		m_actors[data.Name] = PL_Actor(data);
		return true;
	}
	return false;
}

void PL::PL_Library::ClearDeadActors()
{
	std::vector<std::string> m_deadActors;
	for (auto& actor : m_actors)
		if (actor.second.IsAlive())
			m_deadActors.push_back(actor.first);

	for (auto name : m_deadActors)
		m_actors.erase(name);
}

void PL::PL_Library::Save()
{
	json j;

	//Write data to disk
	GetActors(j);
	GetApparel(j);
	GetWeapons(j);
	SaveJSON(j, m_folder + m_domain + PL_DATA_EXT);
}

bool PL::PL_Library::GetActor(std::string name, PL_ActorData& data)
{
	auto found = m_actors.find(name);
	if(found != m_actors.end())
	{
		data = found->second.GetActorData();
		return true;
	}
	return false;
}

bool PL::PL_Library::KillActor(std::string name)
{
	if (m_actors.find(name) != m_actors.end())
	{
		m_actors.erase(name);
		return true;
	}
	return false;
}

bool PL::PL_Library::ProcessJSON(const json & j, bool isAddons)
{
	bool success = false;
	PL_ApparelMap* apparelmap = nullptr;
	PL_WeaponMap* weaponsmap = nullptr;

	//Set approprate map
	if (isAddons)
	{
		apparelmap = &m_apparel;
		weaponsmap = &m_weapons;
	}
	else
	{
		apparelmap = &m_apparel_ext;
		weaponsmap = &m_weapons_ext;
	}

	if (PL::json_contains(j, "Actors") && isAddons == false)
	{
		PL_ActorData actor;
		for (auto& sub : j["Actors"])
		{
			if (actor.LoadJSON(sub))
			{
				CreateActor(actor);
			}
		}
		success = true;
	}
	if (PL::json_contains(j, "Apparel"))
	{
		PL_Apparel apparel;
		for (auto& sub : j["Weapons"])
		{
			if (apparel.LoadJSON(sub))
			{
				CreateApparel(apparel, *apparelmap);
			}
		}
		success = true;
	}
	if (PL::json_contains(j, "Weapons"))
	{
		PL_Weapon weapon;
		for (auto& sub : j["Weapons"])
		{
			if (weapon.LoadJSON(sub))
			{
				CreateWeapon(weapon, *weaponsmap);
			}
		}
		success = true;
	}
	return success;
}

bool PL::PL_Library::CreateWeapon(const PL_Weapon weapon, PL_WeaponMap& map)
{
	auto item = map.find(weapon.GetName());
	if (item == map.end())
	{
		map.emplace(weapon.GetName(), weapon);
		return true;
	}
	return false;
}

bool PL::PL_Library::CreateApparel(const PL_Apparel apparel, PL_ApparelMap& map)
{
	auto item = map.find(apparel.GetName());
	if (item == map.end())
	{
		map.emplace(apparel.GetName(), apparel);
		return true;
	}
	return false;
}

void PL::PL_Library::GetActors(json& j)
{
	for (auto& x : m_actors)
	{
		j["Actors"].push_back(x.second.GetActorData());
	}
}

void PL::PL_Library::GetApparel(json& j)
{
	for (auto& x : m_apparel)
	{
		j["Apparel"].push_back(x.second);
	}
}

void PL::PL_Library::GetWeapons(json& j)
{
	for (auto& x : m_weapons)
	{
		j["Weapons"].push_back(x.second);
	}
}

void PL::PL_Library::LoadAddons()
{
	std::string filename = m_folder + m_domain + PL_SUPPLEMENTARY_EXT;
	std::ifstream file(filename);
	if (file)
	{
		while (!file.eof())
		{
			std::string subFile;
			std::getline(file, subFile);
			json j;
			if (LoadJSON(j, m_folder + subFile))
			{
				ProcessJSON(j, true);
			}
		}
	}
	else
	{
		std::ofstream file(m_folder + m_domain + PL_SUPPLEMENTARY_EXT);
		if (file)
		{
			file << "";
			file.close();
		}
	}
}

bool PL::LoadJSON(json & j, const std::string filename)
{
	std::ifstream file(filename);
	std::string fileRaw;
	if (file)
	{
		file >> j;
		return true;
	}
	return false;
}

void PL::SaveJSON(const json & j, const std::string filename)
{
	std::ofstream file(filename);
	if (file)
	{
		file << std::setw(4) << j << std::endl;
		file.close();
	}
}
