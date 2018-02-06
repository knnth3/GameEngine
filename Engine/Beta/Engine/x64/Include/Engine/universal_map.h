#pragma once
#include <unordered_map>
#include "universal_struct.h"

namespace Engine
{
	/*Container to hold abstract types of data/objects.
	The"universal_struct" class is used to add new data/objects.
	All keys are set to uppercase and max length is 10 characters.*/
	class universal_map
	{
	public:
		__engine_decl bool Insert(const std::string& key, const universal_struct& addon);
		__engine_decl const universal_struct* At(const std::string& key)const;
		__engine_decl const universal_struct* operator[](const std::string& key)const;
		__engine_decl void Remove(const std::string& key);
		__engine_decl void Clear();
		__engine_decl std::vector<std::string> List();
	private:
		std::unordered_map<std::string, const universal_struct> m_internal_data;
	};
}

