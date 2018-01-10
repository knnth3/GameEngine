#pragma once
#ifdef M_PI
#undef M_PI
#endif
#define _USE_MATH_DEFINES
#include <math.h>
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

//OS Types
#define OS_UNDEFINED 0x00
#define OS_WIN32 0x01
#define OS_APPLE 0x02
#define OS_MINGW 0x03
#define OS_LINUX 0x04

#if defined(_WIN32)
#define PLATFORM OS_WIN32
#elif defined(__APPLE__)
#define PLATFORM OS_APPLE
#elif defined(__MINGW32__)
#define PLATFORM OS_MINGW
#elif defined(__linux__)
#define OS_LINUX
#else
#define PLATFORM OS_UNDEFINED
#endif


#if PLATFORM == OS_WIN32
//included libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//Included headers
#include <comdef.h>
#include <D3D11_3.h>
#include "D3Dcompiler.h"
#else
//Stuff for other operating systems
#endif


#ifdef ENGINE_EXPORTS
#define __engine_decl __declspec(dllexport)
#else
#define __engine_decl __declspec(dllimport)
#endif

#define METER 100.0f

namespace Graphics
{

	__engine_decl inline void GetFileExt(const std::string& path, std::string& ext);
	__engine_decl inline std::wstring To_wstr(std::string val);
	__engine_decl inline std::wstring To_wstr(bool val);
	__engine_decl inline std::string To_str(std::wstring val);
	__engine_decl inline std::string To_str(bool val);
	__engine_decl inline std::string To_upper(const std::string& val);

}