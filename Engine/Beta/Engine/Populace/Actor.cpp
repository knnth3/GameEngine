#include "Actor.h"




PL::PL_Actor::PL_Actor(std::string name)
{
	m_name = name;
	m_maxHealth = 100;
	m_currentHealth = m_maxHealth;
}

std::string PL::PL_Actor::GetName()
{
	return m_name;
}

int16_t PL::PL_Actor::GetCurrentHealth()
{
	return m_currentHealth;
}

int16_t PL::PL_Actor::GetMaxHealth()
{
	return m_maxHealth;
}
