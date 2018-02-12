#include <glm\gtx\transform.hpp>
#include "DllSettings.h"


void Graphics::GetFileExt(const std::string& path, std::string & ext)
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