#pragma once
#include "DLLSettings.h"

struct PL_Item_Desc
{
	std::string Name;
	std::string Type;
};

class PL_Item
{
public:
	PL_DLL_API PL_Item();
	PL_DLL_API PL_Item(std::string name, std::string type, std::string desc,
		uint16_t weight, float price);
	PL_DLL_API virtual std::string GetName();
	PL_DLL_API virtual std::string GetDesc();
	PL_DLL_API virtual std::string GetType();
	PL_DLL_API virtual uint16_t GetWeight();
	PL_DLL_API virtual float GetPrice();
	PL_DLL_API virtual PL_Item_Desc GetItemDesc()final;

protected:
	PL_DLL_API virtual void SetName(std::string name)final;
	PL_DLL_API virtual void SetDesc(std::string desc)final;
	PL_DLL_API virtual void SetType(std::string type)final;
	PL_DLL_API virtual void SetWeight(uint16_t weight)final;
	PL_DLL_API virtual void SetPrice(float price)final;

private:

	std::string m_name;
	std::string m_type;
	std::string m_desc;
	uint16_t m_weight;
	float m_price;
};
typedef std::unique_ptr<PL_Item> PL_ItemPtr;
//JSON
void to_json(json& j, const PL_Item_Desc& p);

