#include "PL_Transport.h"

PL::PL_Transport::PL_Transport()
{
	m_name = "";
	m_description = "";
	m_price = 0;
	m_currentWeight = 0;
	m_carryCapacity = 0;
}

PL::PL_Transport::PL_Transport(const PL_TransportInfo info)
{
	m_name = info.Name;
	m_description = info.Description;
	m_price = info.Price;
	m_currentWeight = 0;
	m_carryCapacity = info.CarryCapacity;
}

std::string PL::PL_Transport::GetName()const
{
	return m_name;
}

std::string PL::PL_Transport::GetDescription()const
{
	return m_description;
}

float PL::PL_Transport::GetPrice()const
{
	return m_price;
}

unsigned int PL::PL_Transport::GetCarryCapacity()const
{
	return m_carryCapacity;
}

unsigned int PL::PL_Transport::GetCurrentWeight()const
{
	return m_currentWeight;
}

PL_TransportInfo PL::PL_Transport::GetInfo()const
{
	PL_TransportInfo info;
	info.CarryCapacity = m_carryCapacity;
	info.Description = m_description;
	info.Name = m_name;
	info.Price = m_price;
	return info;
}

bool PL::PL_Transport::LoadJSON(const json & j)
{
	//Make sure the file is valid
	if (!json_contains(j, "Name") ||
	!json_contains(j, "Description") ||
	!json_contains(j, "Price") ||
	!json_contains(j, "Carry Capacity"))
		return false;

	m_name = j["Name"].get<std::string>();
	m_description = j["Description"].get<std::string>();
	m_price = j["Price"].get<float>();
	m_carryCapacity = j["Carry Capacity"].get<unsigned int>();
	return true;
}

bool PL::PL_Transport::Store(PL_Item& item)
{
	if (item.GetWeight() + m_currentWeight <= m_carryCapacity)
	{
		m_inventory.push_back(item);
		return true;
	}
	return false;
}

bool PL::PL_Transport::Retrieve(PL_Item& item, std::string name)
{
	for (int index = 0; index < m_inventory.size(); index++)
	{
		if (m_inventory[index].GetName().compare(name) == 0)
		{
			//Item found
			m_inventory[index] = item;
			return true;
		}
	}
	return false;
}

void PL::to_json(json & j, const PL_Transport & p)
{
	j["Name"] = p.GetName();
	j["Price"] = p.GetPrice();
	j["Carry Capacity"] = p.GetCarryCapacity();
	j["Description"] = p.GetDescription();
}

void PL::from_json(const json & j, PL_Transport & item)
{
	item.LoadJSON(j);
}
