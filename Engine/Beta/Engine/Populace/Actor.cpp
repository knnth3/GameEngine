#include "Actor.h"

PL::PL_Actor::PL_Actor()
{
	m_name = "";
	m_maxHealth = 100;
	m_currentHealth = m_maxHealth;
	m_bAlive = true;
}

PL::PL_Actor::PL_Actor(std::string name)
{
	m_name = name;
	m_maxHealth = 100;
	m_currentHealth = m_maxHealth;
	m_bAlive = true;
}

PL::PL_Actor::PL_Actor(PL_ActorData data)
{
	m_bAlive = data.IsAlive;
	m_currentHealth = data.CurrentHealth;
	m_maxHealth = data.MaxHealth;
	m_name = data.Name;
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

PL_ActorData PL::PL_Actor::GetActorData()
{
	PL_ActorData data;
	if(m_armor)
		data.Armor = m_armor->GetItemDesc();
	if (m_shield)
		data.Shield = m_shield->GetItemDesc();
	if (m_mainHand)
		data.MainHand = m_mainHand->GetItemDesc();
	if (m_offHand)
		data.OffHand = m_offHand->GetItemDesc();
	if(m_mount)
		data.Mount = m_mount->GetInfo();
	data.Name = m_name;
	data.MaxHealth = m_maxHealth;
	data.CurrentHealth = m_currentHealth;
	for (auto& x : m_inventory)
		data.Inventory.push_back(x->GetItemDesc());
	return data;
}

bool PL::PL_Actor::AddItem(const PL_Item item)
{
	//Logic for adding items in here
	//Ex: check for sufficient space/weight capacity
	m_inventory.emplace_back(new PL_Item(item));
	return true;
}

void PL::PL_Actor::AddMount(const PL_Transport mount)
{
	m_transports.emplace_back(new PL_Transport(mount));
}

void PL::PL_Actor::SetLivingStatus(const bool val)
{
	m_bAlive = val;
}

bool PL::PL_Actor::Equip(std::string name, PL_EQUIPMENT_SLOT slot)
{
	for (int index = 0; index < m_inventory.size(); index++)
	{
		if (m_inventory[index]->GetName().compare(name) == 0)
		{
			auto& item = m_inventory[index];
			if (item->GetDesc().compare("Armor") == 0)
			{
				if (slot == PL_SLOT_BODY)
				{
					auto polled = static_cast<PL_Apparel*>(item.release());
					if (m_armor)
						m_inventory.emplace_back(m_armor.release());
					m_armor = PL_ApparelPtr(polled);
					return true;
				}
				else if (slot == PL_SLOT_SHIELD)
				{
					auto polled = static_cast<PL_Apparel*>(item.release());
					if (m_shield)
						m_inventory.emplace_back(m_shield.release());
					m_shield = PL_ApparelPtr(polled);
					return true;
				}
			}
			else if (item->GetDesc().compare("Weapon") == 0)
			{
				if (slot == PL_SLOT_MAINHAND)
				{
					auto polled = static_cast<PL_Weapon*>(item.release());
					if (m_mainHand)
						m_inventory.emplace_back(m_mainHand.release());
					m_mainHand = PL_WeaponPtr(polled);
						return true;
				}
				else if (slot == PL_SLOT_OFFHAND)
				{
					auto polled = static_cast<PL_Weapon*>(item.release());
					if (m_offHand)
						m_inventory.emplace_back(m_offHand.release());
					m_offHand = PL_WeaponPtr(polled);
						return true;
				}
			}
		}
	}
	return false;
}

bool PL::PL_Actor::Mount(std::string name)
{
	for (int index = 0; index < m_transports.size(); index++)
	{
		if (m_transports[index]->GetName().compare(name) == 0)
		{
			m_transports[index].swap(m_mount);
			return true;
		}
	}
	return false;
}

void to_json(json& j, const PL_ActorData & p) 
{
	j = json
	{
		{ "Name", p.Name },
		{ "Max Health", p.MaxHealth },
		{ "Current Health", p.CurrentHealth },
		{ "Alive", p.IsAlive },
	};
	//Extra Information
	if (!p.Inventory.empty())
		j["Inventory"] = p.Inventory;
	if (!p.Armor.Name.empty())
		j["Armor"] = p.Armor;
	if (!p.Armor.Name.empty())
		j["Shield"] = p.Armor;
	if (!p.Shield.Name.empty())
		j["Main-Hand Weapon"] = p.Armor;
	if (!p.MainHand.Name.empty())
		j["Off-Hand Weapon"] = p.Armor;
	if (!p.OffHand.Name.empty())
		j["Mount"] = p.Armor;

}

void from_json(const json & j, PL_ActorData & item)
{
	item.LoadJSON(j);
}

bool PL_ActorData::LoadJSON(const json & j)
{
	//Make sure the file is valid
	if (!PL::json_contains(j, "Name") ||
		!PL::json_contains(j, "Max Health") ||
		!PL::json_contains(j, "Current Health") ||
		!PL::json_contains(j, "Alive"))
		return false;

	Name = j["Name"].get<std::string>();
	MaxHealth = j["Max Health"].get<std::string>();
	m_price = j["Current Health"].get<float>();
	al = j["Alive"].get<unsigned int>();
	return true;
}
