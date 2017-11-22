#include "PL_Item.h"

void to_json(json & j, const PL_Item & p)
{
	j = json
	{
		{ "Name", p.Name },
		{ "Weight", p.Weight },
		{ "Type", p.Type }
	};
}

PL_Item::PL_Item(std::string type)
{
	Type = type;
}
