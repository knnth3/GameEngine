#include "Game1.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int val = -1;
	Game1 window("My Application", 800.0f, 600.0f);
	if (window.Initialize())
	{
		val = window.Run();
	}
	return val;
}