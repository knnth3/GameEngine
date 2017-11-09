#pragma once
#include <string>
#include <memory>


#ifdef SYSTEM_EXPORTS
#define AppDLL_API __declspec(dllexport)
#else
#define AppDLL_API __declspec(dllimport)
#endif

//OS Types
#define OS_UNDEFINED 0x00
#define OS_WINDOWS 0x01
#define OS_APPLE 0x02
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