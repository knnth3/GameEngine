#include "PL_Consumable.h"

PL::PL_Consumable::PL_Consumable()
{
	m_bIsCooked = false;
	m_tasteLevel = 0;
	m_healthBonus = 0;
	m_portions = 0;
	m_saying = "";
}

PL::PL_Consumable::PL_Consumable(bool cooked, int tasteLevel, int healthBonus)
{
	m_bIsCooked = cooked;
	m_tasteLevel = tasteLevel;
	m_healthBonus = healthBonus;
	m_portions = 0;
	m_saying = "";
}

void PL::PL_Consumable::SetSaying(std::string saying)
{
	m_saying = saying;
}

void PL::PL_Consumable::SetNumPortions(unsigned int portions)
{
	m_portions = portions;
}

std::string PL::PL_Consumable::GetSaying()
{
	return m_saying;
}

unsigned int PL::PL_Consumable::GetNumPortions()
{
	return m_portions;
}

bool PL::PL_Consumable::ConsumePortion(unsigned int portions)
{
	if (m_portions > portions)
	{
		m_portions -= portions;
		return true;
	}
	return false;
}

bool PL::PL_Consumable::Consumed()
{
	if (m_portions == 0)
		return true;
	return false;
}
