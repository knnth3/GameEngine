#pragma once
#include "PL_Apparel.h"
#include "PL_Transport.h"
#include "PL_Weapon.h"
typedef std::unique_ptr<PL::PL_Apparel> PL_ApparelPtr;
typedef std::unique_ptr<PL::PL_Weapon> PL_WeaponPtr;
typedef std::unique_ptr<PL::PL_Transport> PL_TransportPtr;

enum PL_EQUIPMENT_SLOT
{
	PL_SLOT_BODY,
	PL_SLOT_SHIELD,
	PL_SLOT_MAINHAND,
	PL_SLOT_OFFHAND
};

struct PL_ActorData
{
	bool LoadJSON(const json& j);

	bool IsAlive;
	std::string Name;
	int16_t MaxHealth;
	int16_t CurrentHealth;
	PL_Item_Desc Armor;
	PL_Item_Desc Shield;
	PL_Item_Desc MainHand;
	PL_Item_Desc OffHand;
	PL_TransportInfo Mount;
	std::vector<PL_Item_Desc> Inventory;
};

namespace PL
{

	class PL_Actor
	{
	public:
		PL_Actor();
		PL_Actor(std::string name);
		PL_Actor(PL_ActorData data);

		//Get Functions
		std::string GetName();
		int16_t GetCurrentHealth();
		int16_t GetMaxHealth();
		bool IsAlive();
		PL_ActorData GetActorData();

		//Set Functions
		bool AddItem(const PL_Item item);
		void AddMount(const PL_Transport mount);
		void SetLivingStatus(const bool val);
		bool Equip(std::string name, PL_EQUIPMENT_SLOT slot);
		bool Mount(std::string name);

	private:

		bool m_bAlive;
		std::string m_name;
		int16_t m_maxHealth;
		int16_t m_currentHealth;
		PL_ApparelPtr m_armor;
		PL_ApparelPtr m_shield;
		PL_WeaponPtr m_mainHand;
		PL_WeaponPtr m_offHand;
		PL_TransportPtr m_mount;
		std::vector<PL_TransportPtr> m_transports;
		std::vector<PL_ItemPtr> m_inventory;

	};
}

//JSON
void to_json(json& j, const PL_ActorData& p);
void from_json(const json& j, PL_ActorData& item);