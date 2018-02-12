#pragma once
#include "PL_Item.h"


struct PL_TransportInfo
{
	std::string Name;
	std::string Description;
	float Price;
	unsigned int CarryCapacity;
};

namespace PL
{
	class PL_Transport
	{
	public:
		PL_Transport();
		PL_Transport(const PL_TransportInfo info);

		//Get Functions
		std::string GetName()const;
		std::string GetDescription()const;
		float GetPrice()const;
		unsigned int GetCarryCapacity()const;
		unsigned int GetCurrentWeight()const;
		PL_TransportInfo GetInfo()const;

		//Set Functions
		bool LoadJSON(const json& j);
		bool Store(PL_Item& item);
		bool Retrieve(PL_Item& item, std::string name);

	private:
		std::string m_name;
		std::string m_description;
		float m_price;
		unsigned int m_currentWeight;
		unsigned int m_carryCapacity;
		std::vector<PL_Item> m_inventory;
	};

	void to_json(json& j, const PL_Transport& p);
	void from_json(const json& j, PL_Transport& item);
}