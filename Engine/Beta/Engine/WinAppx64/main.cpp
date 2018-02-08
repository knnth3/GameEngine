#include "WindowApp.h"
#include <Windows.h>

using namespace Engine;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int val = -1;
	WindowApp window("My Application", 800.0f, 600.0f);
	if (window.Initialize())
	{
		val = window.Run();
	}
	return val;
}