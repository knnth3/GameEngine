#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <glm\glm.hpp>


//For later use
////API Types
//#define API_NONE 0x00
//#define API_DIRECTX 0x01
//#define API_OPENGL 0x02
//#define API_INUSE API_NONE
#define PI 3.1415927f
#undef DrawText

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
#endif


#ifdef Engine_EXPORTS
#define AppDLL_API __declspec(dllexport)
#else
#define AppDLL_API __declspec(dllimport)
#endif


namespace Lime
{
	class DiffuseLight
	{
	public:
		AppDLL_API DiffuseLight() = default;
		AppDLL_API void SetDirection(const glm::vec3 dir);
		AppDLL_API void SetColor(const glm::vec4 color);
		AppDLL_API const glm::vec3 GetDirection() const;
		AppDLL_API const glm::vec4 GetColor() const;
	private:
		glm::vec3 m_direction;
		glm::vec4 m_color;
	};

}