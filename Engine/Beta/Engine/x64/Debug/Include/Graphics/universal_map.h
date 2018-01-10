#pragma once
#include <unordered_map>
#include "universal_struct.h"

namespace Graphics
{
	/*Container to hold abstract types of data/objects.
	The"universal_struct" class is used to add new data/objects.
	All keys are set to uppercase and max length is 10 characters.*/
	class universal_map
	{
	public:
		GRAPHICS_DLL_API bool Insert(const std::string& key, const universal_struct& addon);
		GRAPHICS_DLL_API const universal_struct* At(const std::string& key)const;
		GRAPHICS_DLL_API const universal_struct* operator[](const std::string& key)const;
		GRAPHICS_DLL_API void Remove(const std::string& key);
		GRAPHICS_DLL_API void Clear();
		GRAPHICS_DLL_API std::vector<std::string> List();
	private:
		std::unordered_map<std::string, const universal_struct> m_internal_data;
	};
}

