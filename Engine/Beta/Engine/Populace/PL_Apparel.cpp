#include "PL_Apparel.h"

PL::PL_Apparel::PL_Apparel()
{
	m_armorClass = PL_Invalid;
	m_armorBonus = 0;
	m_enhancementBonus = 0;
	m_checkModifier = 0;
	m_speedModifier = 0;
}

PL::PL_Apparel::PL_Apparel(PL_AC armorClass, unsigned int armorBonus, unsigned int enhancementBonus, int checkModifier, int speedModifier, bool isShield)
{
	m_armorClass = armorClass;
	m_armorBonus = armorBonus;
	m_enhancementBonus = enhancementBonus;
	m_checkModifier = checkModifier;
	m_speedModifier = speedModifier;
	m_bIsShield = isShield;
}

unsigned int PL::PL_Apparel::GetArmorBonus()const
{
	return m_armorBonus;
}

unsigned int PL::PL_Apparel::GetEnhancementBonus()const
{
	return m_enhancementBonus;
}

int PL::PL_Apparel::GetCheckModifier()const
{
	return m_checkModifier;
}

int PL::PL_Apparel::GetSpeedModifier()const
{
	return m_speedModifier;
}

bool PL::PL_Apparel::IsShield() const
{
	return m_bIsShield;
}

bool PL::PL_Apparel::LoadJSON(const json & j)
{
	return false;
}

void PL::PL_Apparel::SetArmorBonus( unsigned int armor )
{
	m_armorBonus = armor;
}

void PL::to_json(json & j, const PL_Apparel & p)
{
}

void PL::from_json(const json & j, PL_Apparel & item)
{
}
