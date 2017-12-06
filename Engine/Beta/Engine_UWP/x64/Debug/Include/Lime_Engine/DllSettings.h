#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <Graphics\MeshLoader.h>
#include <Graphics\Graphics.h>


namespace LIME_ENGINE
{


#ifdef LIME_ENGINE_EXPORTS
#define ENGINE_DLL_API __declspec(dllexport)
#else
#define ENGINE_DLL_API __declspec(dllimport)
#endif

	enum ENGINE_COLORS
	{
		LOG_WHITE = 0,
		LOG_BLACK,
		LOG_RED,
		LOG_BLUE,
		LOG_GREEN
	};
}

