#pragma once
#include <string>
#include <Windows.h>

#ifdef POPULACE_EXPORTS
#define PL_DLL_API __declspec(dllexport)
#else
#define PL_DLL_API __declspec(dllimport)
#endif

namespace PL
{
	PL_DLL_API inline std::wstring ToWstring(const std::string str)
	{
		// room for 100 characters
		std::vector<WCHAR> buffer(100);
		MultiByteToWideChar(
			CP_ACP,               // code page
			MB_PRECOMPOSED,       // character-type options
			str.c_str(),          // string to map
			(int)str.length(),         // number of bytes in string
			&buffer[0],           // wide-character buffer (must use vector here!)
			100                   // size of buffer
		);
		return std::wstring(buffer.begin(), buffer.end());
	}
}