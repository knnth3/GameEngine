#pragma once
#include <string>
#include <JSON\json.hpp>
using nlohmann::json;

struct PL_Item
{
	PL_Item() = default;
	PL_Item(std::string type);

	std::string Name;
	std::string Type;
	uint16_t Weight;
};

//JSON
void to_json(json& j, const PL_Item& p);

