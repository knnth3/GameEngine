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
		unsigned int GetDurability()const;
		bool IsBroken()const;
		bool IsOneHanded()const;

		//SetFunctions
		void Wear(unsigned int durabilty);
		void Break();
		void Fix();

	private:

		bool m_bOneHanded;
		unsigned int m_damage;
		unsigned int m_durability;
		unsigned int m_currentDurability;
	};

	void to_json(json& j, const PL_Weapon& p);
	void from_json(const json& j, PL_Weapon& item);
}
