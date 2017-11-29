#include "PL_Library.h"
#include <fstream>
#include <iostream>

std::map<std::string, PL::PL_Apparel> PL::PL_Library::m_apparel;
std::map<std::string, PL::PL_Weapon> PL::PL_Library::m_weapons;
std::map<std::string, PL::PL_Actor> PL::PL_Library::m_actors;
std::map<std::string, PL::PL_Transport> PL::PL_Library::m_transportations;
std::string PL::PL_Library::m_folder;
bool PL::PL_Library::Initialize(const std::string folder)
{
	m_folder = folder;
	if (!CreateDir(folder + PL_WEAPON_DIR))
		return false;
	if (!CreateDir(folder + PL_APPAREL_DIR))
		return false;
	if (!CreateDir(folder + PL_ACTOR_DIR))
		return false;

	LoadSettings("Weapons");
	LoadSettings("Apparel");
	LoadSettings("Actors");
	return true;
}

bool PL::PL_Library::GiveMount(std::string actor, std::string item)
{
	auto found = m_transportations.find(item);
	if (found != m_transportations.end())
	{
		m_actors[actor].AddMount(m_transportations.at(item));
		return true;
	}
	else
	{
		std::ifstream file(m_folder + item + ".json");
		if (!file)
			return false;
		json j;
		file >> j;
		file.close();
		m_transportations[item] = j;
		m_actors[actor].AddMount(m_transportations.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::GiveApparel(std::string actor, std::string item)
{
	auto found = m_apparel.find(item);
	if (found != m_apparel.end())
	{
		m_actors[actor].AddItem(m_apparel.at(item));
		return true;
	}
	else
	{
		std::ifstream file(m_folder + item + ".json");
		if (!file)
			return false;
		json j;
		file >> j;
		file.close();
		m_apparel[item] = j;
		m_actors[actor].AddItem(m_apparel.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::GiveWeapon(std::string actor, std::string item)
{
	auto found = m_weapons.find(item);
	if (found != m_weapons.end())
	{
		m_actors[actor].AddItem(m_weapons.at(item));
		return true;
	}
	else
	{
		std::ifstream file(m_folder + item + ".json");
		if (!file)
			return false;
		json j;
		file >> j;
		file.close();
		m_weapons[item] = j;
		m_actors[actor].AddItem(m_weapons.at(item));
		return true;
	}
	return false;
}

bool PL::PL_Library::CreateActor(std::string name)
{
	auto found = m_actors.find(name);
	if (found == m_actors.end())
	{
		m_actors[name] = PL_Actor(name);
	}
	return false;
}

bool PL::PL_Library::CreateActor(PL_ActorData data)
{
	auto found = m_actors.find(data.Name);
	if (found == m_actors.end())
	{
		found->second = PL_Actor(data);
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
	//Write all References to disk
	std::ofstream file(m_folder + "Weapons.settings");
	for (auto x : m_weapons)
		file << x.second.GetName() + ".json" << std::endl;
	file.close();

	file = std::ofstream(m_folder + "Apparel.settings");
	for (auto x : m_apparel)
		file << x.second.GetName() + ".json" << std::endl;
	file.close();

	file = std::ofstream(m_folder + "Actors.settings");
	for (auto& x : m_actors)
		file << x.second.GetName() + ".json" << std::endl;
	file.close();

	//Write actors to disk
	SaveActors();
	//Write apparel to disk
	SaveApparel();
	//Write weapons to disk
	SaveWeapons();
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

void PL::PL_Library::SaveActors()
{
	for (auto& x : m_actors)
	{
		json j;
		j[x.second.GetName()] = x.second.GetActorData();
		std::ofstream file(m_folder + PL_ACTOR_DIR + x.second.GetName() +".json");
		if (file)
		{
			file << j << std::endl;
			file.close();
		}
	}
}

void PL::PL_Library::SaveApparel()
{
	for (auto& x : m_apparel)
	{
		json j;
		j[x.second.GetName()] = x.second;
		std::ofstream file(m_folder + PL_APPAREL_DIR + x.second.GetName() + ".json");
		if (file)
		{
			file << j << std::endl;
			file.close();
		}
	}
}

void PL::PL_Library::SaveWeapons()
{
	for (auto& x : m_weapons)
	{
		json j;
		j[x.second.GetName()] = x.second;
		std::ofstream file(m_folder + PL_WEAPON_DIR + x.second.GetName() + ".json");
		if (file)
		{
			file << j << std::endl;
			file.close();
		}
	}
}

void PL::PL_Library::LoadSettings(std::string type)
{
	std::string filename = m_folder + type + ".settings";
	std::ifstream file(filename);
	if (!file)
	{
		std::ofstream o(filename);
		o << "" << std::endl;
		o.close();
	}
	else
	{
		while (!file.eof())
		{
			std::string ext;
			std::getline(file, ext);
			if (type.compare("Weapons") == 0)
			{
				LoadActor(m_folder + PL_ACTOR_DIR + ext);
			}
			else if (type.compare("Apparel") == 0)
			{
				LoadApparel(m_folder + PL_APPAREL_DIR + ext);
			}
			else if (type.compare("Actors") == 0)
			{
				LoadWeapon(m_folder + PL_WEAPON_DIR + ext);
			}
		}
	}
}

void PL::PL_Library::LoadActor(std::string filepath)
{
	json j;
	std::ifstream file(filepath);
	if (file)
	{
		file >> j;
		PL_ActorData actor = j;
		CreateActor(actor);
	}
}

void PL::PL_Library::LoadWeapon(std::string filepath)
{
}

void PL::PL_Library::LoadApparel(std::string filepath)
{
}
