#include <glm\gtx\transform.hpp>
#include <algorithm>
#include "DllSettings.h"


void Engine::GetFileExt(const std::string& path, std::string & ext)
{
	//removes the '.'
	size_t extPoint = path.find_last_of('.') + 1;

	try
	{
		if (extPoint)
			ext = path.substr(extPoint, path.size() - 1);
	}
	catch (...) //General (catch all)
	{
		//Log error
		ext = "";
	}
}

std::wstring Engine::To_wstr(std::string val)
{
	std::wstring temp;
	temp.insert(temp.begin(), val.begin(), val.end());
	return temp;
}

std::wstring Engine::To_wstr(bool val)
{
	if (val)
		return L"True";

	else
		return L"False";
}

std::string Engine::To_str(std::wstring val)
{
	std::string temp;
	temp.insert(temp.begin(), val.begin(), val.end());
	return temp;
}

std::string Engine::To_str(bool val)
{
	if (val)
		return "True";

	else
		return "False";
}

std::string Engine::To_upper(const std::string & val)
{
	std::string str = val;
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}
