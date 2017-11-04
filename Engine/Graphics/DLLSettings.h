#pragma once
#define METER 100.0f
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include <string>
#include <vector>
#include <glm\glm.hpp>

#ifdef GRAPHICS_EXPORTS
#define AppDLL_API __declspec(dllexport)
#else
#define AppDLL_API __declspec(dllimport)
#endif


static void GetFileExt(const std::string& path, std::string & ext)
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