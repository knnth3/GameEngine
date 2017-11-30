#include "PL_Weapon.h"

PL::PL_Weapon::PL_Weapon()
{
	m_bOneHanded = true;
	m_damage = 0;
}

PL::PL_Weapon::PL_Weapon(bool oneHnaded, unsigned int damage, unsigned int durability)
{
	m_bOneHanded = oneHnaded;
	m_damage = damage;
}

unsigned int PL::PL_Weapon::GetDamage()const
{
	return m_damage;
}

bool PL::PL_Weapon::IsOneHanded()const
{
	return m_bOneHanded;
}

bool PL::PL_Weapon::LoadJSON(const json & j)
{
	//Make sure the file is valid
	if (!PL::json_contains(j, "One-Handed") ||
		!PL::json_contains(j, "Damage"))
		return false;

	if (!this->PL_Item::LoadJSON(j))
		return false;

	m_bOneHanded = j["One-Handed"].get<bool>();
	m_damage = j["Damage"].get<unsigned int>();

	return true;
}

void PL::to_json(json & j, const PL_Weapon & p)
{
	j = p.GetBaseJSON();
	j["One-Handed"] = p.IsOneHanded();
	j["Damage"] = p.GetDamage();
}

void PL::from_json(const json & j, PL_Weapon & item)
{
	item.LoadJSON(j);
}
