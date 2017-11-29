#pragma once
#include "Actor.h"
#include "DLLSettings.h"

#define PL_DATA_EXT ".json"
#define PL_SUPPLEMENTARY_EXT ".addons"

namespace PL
{
	typedef std::map<std::string, PL_Weapon> PL_WeaponMap;
	typedef std::map<std::string, PL_Apparel> PL_ApparelMap;
	typedef std::map<std::string, PL_Actor> PL_ActorMap;

	class PL_Library
	{
	public:
		static bool Initialize(const std::string folder, const std::string domain);
		static bool GiveMount(const std::string actor, const std::string item);
		static bool GiveApparel(const std::string actor, const std::string item);
		static bool GiveWeapon(const std::string actor, const std::string item);
		static bool CreateActor(const std::string name);
		static bool CreateActor(const PL_ActorData data);
		static void ClearDeadActors();
		static void Save();

		//Get Functions
		static bool GetActor(std::string name, PL_ActorData& data);
		static bool KillActor(std::string name);

	private:
		static bool ProcessJSON(const json& j, bool isAddon = false);
		static bool CreateWeapon(const PL_Weapon weapon, PL_WeaponMap& map);
		static bool CreateApparel(const PL_Apparel apparel, PL_ApparelMap& map);
		static void GetActors(json& j);
		static void GetApparel(json& j);
		static void GetWeapons(json& j);
		static void LoadAddons();

		static std::string m_domain;
		static std::string m_folder;

		//Base data
		static PL_ApparelMap m_apparel;
		static PL_WeaponMap m_weapons;
		static PL_ActorMap m_actors;

		//Add-ons
		static PL_ApparelMap m_apparel_ext;
		static PL_WeaponMap m_weapons_ext;

		static std::map<std::string, PL_Transport> m_transportations;
	};

	bool LoadJSON(json& j, const std::string filename);
	void SaveJSON(const json& j, const std::string filename);
}

