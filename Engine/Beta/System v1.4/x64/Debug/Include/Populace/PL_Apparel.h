#pragma once
#include "PL_Item.h"

namespace PL
{
	enum PL_AC
	{
		PL_Invalid = 0,
		PL_Light,
		PL_Heavy
	};

	class PL_Apparel:
		public PL_Item
	{
	public:
		PL_Apparel();
		PL_Apparel(PL_AC armorClass, unsigned int armorBonus, unsigned int enhancementBonus,
			int checkModifier, int speedModifier, bool isShield = false);

		//Get Functions
		unsigned int GetArmorBonus()const;
		unsigned int GetEnhancementBonus()const;
		int GetCheckModifier()const;
		int GetSpeedModifier()const;
		bool IsShield()const;

		//Set Functions
		bool LoadJSON(const json& j);
		void SetArmorBonus(unsigned int armor);



	private:

		PL_AC m_armorClass;
		bool m_bIsShield;
		unsigned int m_armorBonus;
		unsigned int m_enhancementBonus;
		int m_checkModifier;
		int m_speedModifier;

	};


	void to_json(json& j, const PL_Apparel& p);
	void from_json(const json& j, PL_Apparel& item);
}