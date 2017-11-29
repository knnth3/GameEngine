#pragma once
#include "Actor.h"
#include "DLLSettings.h"

#define PL_WEAPON_DIR "Weapons\\"
#define PL_APPAREL_DIR "Apparel\\"
#define PL_ACTOR_DIR "Actors\\"

namespace PL
{
	class PL_Library
	{
	public:
		static bool Initialize(const std::string folder);
		static bool GiveMount(std::string actor, std::string item);
		static bool GiveApparel(std::string actor, std::string item);
		static bool GiveWeapon(std::string actor, std::string item);
		static bool CreateActor(std::string name);
		static bool CreateActor(PL_ActorData data);
		static void ClearDeadActors();
		static void Save();

		//Get Functions
		static bool GetActor(std::string name, PL_ActorData& data);
		static bool KillActor(std::string name);

	private:
		static void SaveActors();
		static void SaveApparel();
		static void SaveWeapons();
		static void LoadSettings(std::string type);
		static void LoadActor(std::string filepath);
		static void LoadWeapon(std::string filepath);
		static void LoadApparel(std::string filepath);

		static std::string m_folder;
		static std::map<std::string, PL_Apparel> m_apparel;
		static std::map<std::string, PL_Weapon> m_weapons;
		static std::map<std::string, PL_Actor> m_actors;
		static std::map<std::string, PL_Transport> m_transportations;
	};

	void SaveJSON(const json& j, const std::string filename);
}

