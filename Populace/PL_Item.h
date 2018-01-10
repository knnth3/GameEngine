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
	PL_DLL_API virtual std::string GetName()const;
	PL_DLL_API virtual std::string GetDesc()const;
	PL_DLL_API virtual std::string GetType()const;
	PL_DLL_API virtual uint16_t GetWeight()const;
	PL_DLL_API virtual float GetPrice()const;
	PL_DLL_API virtual PL_Item_Desc GetItemDesc()const final;
	PL_DLL_API virtual json GetBaseJSON()const final;
	PL_DLL_API virtual bool LoadJSON(const json& j);

protected:
	PL_DLL_API virtual void SetName(std::string name)final;
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
void from_json(const json& j, PL_Item_Desc& item);

