#include "PL_Weapon.h"

PL::PL_Weapon::PL_Weapon()
{
	m_bOneHanded = true;
	m_damage = 0;
	m_durability = 0;
	m_currentDurability = 0;
}

PL::PL_Weapon::PL_Weapon(bool oneHnaded, unsigned int damage, unsigned int durability)
{
	m_bOneHanded = oneHnaded;
	m_damage = damage;
	m_durability = durability;
	m_currentDurability = m_durability;
}

unsigned int PL::PL_Weapon::GetDamage()const
{
	if (m_currentDurability == 0)
		return 0;
	return m_damage;
}

unsigned int PL::PL_Weapon::GetDurability()const
{
	return m_currentDurability;
}

bool PL::PL_Weapon::IsBroken()const
{
	if (m_currentDurability == 0)
		return true;
	return false;
}

bool PL::PL_Weapon::IsOneHanded()const
{
	return m_bOneHanded;
}

void PL::PL_Weapon::Wear(unsigned int durabilty)
{
	m_currentDurability -= durabilty;
}

void PL::PL_Weapon::Break()
{
	m_currentDurability = 0;
}

void PL::PL_Weapon::Fix()
{
	m_currentDurability = m_durability;
}

void PL::to_json(json & j, const PL_Weapon & p)
{
}

void PL::from_json(const json & j, PL_Weapon & item)
{
}
