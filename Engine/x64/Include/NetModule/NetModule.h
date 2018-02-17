#pragma once

#include <iostream>

//Export Macro
#ifdef NetModule_EXPORTS
#define NET_API __declspec(dllexport)
#else
#define NET_API __declspec(dllimport)
#endif


// platform detection
#define NET_PLATFORM_WINDOWS  1
#define NET_PLATFORM_MAC      2
#define NET_PLATFORM_UNIX     3

#if defined(_WIN32)
#define NET_PLATFORM NET_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define NET_PLATFORM NET_PLATFORM_MAC
#else
#define NET_PLATFORM NET_PLATFORM_UNIX
#endif

#if NET_PLATFORM == NET_PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#endif

#if NET_PLATFORM == NET_PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#endif

#include <stdio.h>


inline bool InitializeSockets()
{
#if NET_PLATFORM == NET_PLATFORM_WINDOWS
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2),
		&WsaData)
		== NO_ERROR;
#else
	return true;
#endif
}

inline void ShutdownSockets()
{
#if NET_PLATFORM == NET_PLATFORM_WINDOWS
	WSACleanup();
#endif
}


//Constants
#define NET_PORT 57232
#define CONNECTION_KEY (Identification)5274