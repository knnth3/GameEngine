#pragma once
#include "PL_Item.h"


namespace PL
{
	class PL_Actor
	{
	public:
		PL_Actor(std::string name);

		//Get Functions
		std::string GetName();
		int16_t GetCurrentHealth();
		int16_t GetMaxHealth();
		bool IsAlive();
		const std::vector<PL_Item> GetInventory();
		void AddItem(const PL_Item item);

		//Set Functions
		void SetLivingStatus(const bool val);

	private:

		bool m_bAlive;
		std::string m_name;
		int16_t m_maxHealth;
		int16_t m_currentHealth;
		std::vector<PL_Item> m_inventory;

	};
}

struct PL_ActorData
{
	PL_ActorData() = default;
	PL_ActorData(PL::PL_Actor& actor);

	bool IsAlive;
	std::string Name;
	int16_t MaxHealth;
	int16_t CurrentHealth;
	std::vector<PL_Item> Inventory;
};

//JSON
void to_json(json& j, const PL_ActorData& p);