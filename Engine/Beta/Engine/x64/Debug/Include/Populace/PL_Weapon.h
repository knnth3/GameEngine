#pragma once
#include "PL_Item.h"



namespace PL
{

	class PL_Weapon:
		public PL_Item
	{
	public:
		PL_Weapon();
		PL_Weapon(bool oneHnaded, unsigned int m_damage, unsigned int m_durability);

		//Get Functions
		unsigned int GetDamage()const;
		bool IsOneHanded()const;

		//SetFunctions
		bool LoadJSON(const json& j);

	private:

		bool m_bOneHanded;
		unsigned int m_damage;
	};

	void to_json(json& j, const PL_Weapon& p);
	void from_json(const json& j, PL_Weapon& item);
}
