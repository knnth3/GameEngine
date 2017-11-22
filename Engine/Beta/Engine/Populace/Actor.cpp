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

bool PL::PL_Actor::IsAlive()
{
	return m_bAlive;
}

const std::vector<PL_Item> PL::PL_Actor::GetInventory()
{
	return m_inventory;
}

void PL::PL_Actor::AddItem(const PL_Item item)
{
	//Logic for adding items in here
	//Ex: check for sufficient space/weight capacity
	m_inventory.push_back(item);
}

void PL::PL_Actor::SetLivingStatus(const bool val)
{
	m_bAlive = val;
}

PL_ActorData::PL_ActorData(PL::PL_Actor & actor)
{
	Name = actor.GetName();
	MaxHealth = actor.GetMaxHealth();
	CurrentHealth = actor.GetCurrentHealth();
	Inventory = actor.GetInventory();
}

void to_json(json& j, const PL_ActorData & p) {
	j = json
	{
		{ "Name", p.Name },
		{ "Max Health", p.MaxHealth },
		{ "Current Health", p.CurrentHealth },
		{ "Alive", p.IsAlive },
		{ "Inventory", p.Inventory }
	};
}