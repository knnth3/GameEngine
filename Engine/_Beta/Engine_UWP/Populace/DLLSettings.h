#pragma once
#ifdef POPULACE_EXPORTS
#define PL_DLL_API __declspec(dllexport)
#else
#define PL_DLL_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <Windows.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <JSON\json.hpp>

using json = nlohmann::json;

namespace PL
{
	//Helper Functions

	PL_DLL_API inline bool json_contains(const json& j, std::string search)
	{
		auto& found = j.find(search);
		if (found != j.end())
			return true;
		return false;
	}

	PL_DLL_API inline std::wstring ToWstring(const std::string str)
	{
		// room for 100 characters
		std::vector<WCHAR> buffer(str.size());
		MultiByteToWideChar(
			CP_ACP,               // code page
			MB_PRECOMPOSED,       // character-type options
			str.c_str(),          // string to map
			(int)str.length(),    // number of bytes in string
			&buffer[0],           // wide-character buffer (must use vector here!)
			(int)str.size()       // size of buffer
		);
		return std::wstring(buffer.begin(), buffer.end());
	}

	PL_DLL_API inline std::string ToString(const std::wstring str)
	{
		// room for 100 characters
		std::vector<CHAR> buffer(str.size());
		WideCharToMultiByte(
			CP_ACP,               // code page
			WC_COMPOSITECHECK,    // character-type options
			str.c_str(),          // string to map
			(int)str.length(),    // number of bytes in string
			&buffer[0],           // wide-character buffer (must use vector here!)
			(int)str.size(),      // size of buffer
			NULL,
			NULL
		);
		return std::string(buffer.begin(), buffer.end());
	}

	PL_DLL_API inline bool CreateDir(const std::string str)
	{
		std::wstring dir = PL::ToWstring(str);
		if (!CreateDirectory(dir.c_str(), NULL) &&
			ERROR_ALREADY_EXISTS != GetLastError())
			return false;
		return true;
	}
}