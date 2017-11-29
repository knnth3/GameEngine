#include "PL_Item.h"

void to_json(json & j, const PL_Item_Desc & p)
{
	j = json
	{
		{ "Name", p.Name },
		{ "Type", p.Type }
	};
}

PL_Item::PL_Item()
{
	m_name = "";
	m_type = "";
	m_desc = "";
	m_weight = 0;
	m_price = 0;
}

PL_Item::PL_Item(std::string name, std::string type, std::string desc, uint16_t weight, float price)
{
	m_name = name;
	m_type = type;
	m_desc = desc;
	m_weight = weight;
	m_price = price;
}

std::string PL_Item::GetName()
{
	return m_name;
}

std::string PL_Item::GetDesc()
{
	return m_desc;
}

std::string PL_Item::GetType()
{
	return m_type;
}

uint16_t PL_Item::GetWeight()
{
	return m_weight;
}

float PL_Item::GetPrice()
{
	return m_price;
}

PL_Item_Desc PL_Item::GetItemDesc()
{
	PL_Item_Desc desc;
	desc.Name = m_name;
	desc.Type = m_type;
	return desc;
}

void PL_Item::SetName(std::string name)
{
	m_name = name;
}

void PL_Item::SetDesc(std::string desc)
{
	m_desc = desc;
}

void PL_Item::SetType(std::string type)
{
	m_type = type;
}

void PL_Item::SetWeight(uint16_t weight)
{
	m_weight = weight;
}

void PL_Item::SetPrice(float price)
{
	m_price = price;
}
