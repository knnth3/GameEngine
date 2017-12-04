#include "PL_Item.h"


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

std::string PL_Item::GetName()const
{
	return m_name;
}

std::string PL_Item::GetDesc()const
{
	return m_desc;
}

std::string PL_Item::GetType()const
{
	return m_type;
}

uint16_t PL_Item::GetWeight()const
{
	return m_weight;
}

float PL_Item::GetPrice()const
{
	return m_price;
}

PL_Item_Desc PL_Item::GetItemDesc()const
{
	PL_Item_Desc desc;
	desc.Name = m_name;
	desc.Type = m_type;
	return desc;
}

json PL_Item::GetBaseJSON()const
{
	json j;
	j["Name"] = GetName();
	j["Type"] = GetType();
	j["Description"] = GetDesc();
	j["Weight"] = GetWeight();
	j["Price"] = GetPrice();
	return j;
}

bool PL_Item::LoadJSON(const json & j)
{
	//Make sure the file is valid
	if (!PL::json_contains(j, "Name") ||
		!PL::json_contains(j, "Type") ||
		!PL::json_contains(j, "Description") ||
		!PL::json_contains(j, "Weight") ||
		!PL::json_contains(j, "Price"))
		return false;

	m_name = j["Name"].get<std::string>();
	m_type = j["Type"].get<std::string>();
	m_desc = j["Description"].get<std::string>();
	m_weight = j["Weight"].get<uint16_t>();
	m_price = j["Price"].get<float>();
	return true;
}

void PL_Item::SetName(std::string name)
{
	m_name = name;
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

void to_json(json & j, const PL_Item_Desc & p)
{
	j = json
	{
		{ "Name", p.Name },
		{ "Type", p.Type }
	};
}

void from_json(const json & j, PL_Item_Desc & item)
{
	item.Name = j["Name"].get<std::string>();
	item.Type = j["Type"].get<std::string>();
}