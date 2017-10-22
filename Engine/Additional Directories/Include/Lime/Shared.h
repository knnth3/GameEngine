#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <glm\glm.hpp>

//For later use
////API Types
//#define API_NONE 0x00
//#define API_DIRECTX 0x01
//#define API_OPENGL 0x02
//#define API_INUSE API_NONE
#define PI 3.1415927f

//OS Types
#define OS_UNDEFINED 0x00
#define OS_WINDOWS 0x01
#define OS_APPLE 0x02
#define OS_MINGW 0x03
#define OS_LINUX 0x04

#if defined(_WIN32)
#define PLATFORM OS_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM OS_APPLE
#elif defined(__MINGW32__)
#define PLATFORM OS_MINGW
#elif defined(__linux__)
#define OS_LINUX
#else
#define PLATFORM OS_UNDEFINED
#endif


#if PLATFORM == OS_WINDOWS
//included libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//Included headers
#include <comdef.h>
#include <d3d11.h>
#include "WindowsAdditionals.h"
#include "D3Dcompiler.h"
#else
//Stuff for other operating systems
#endif


#ifdef Engine_EXPORTS
#define AppDLL_API __declspec(dllexport)
#else
#define AppDLL_API __declspec(dllimport)
#endif


typedef short MeshID;
typedef const uint32_t c_uint;
namespace Lime
{


	void GetFileExt(const std::string& path, std::string& ext);


	class WorldLight
	{
	public:
		WorldLight();
		glm::vec3 m_direction;
		glm::vec4 m_specularColor;
		glm::vec4 m_ambientColor;
		glm::vec4 m_diffuseColor;
		float m_specularPower;
	private:
	};

}