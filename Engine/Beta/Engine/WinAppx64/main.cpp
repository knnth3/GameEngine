#include "WindowApp.h"
#include <Windows.h>
#include <Graphics\universal_map.h>

using namespace Graphics;


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int val = -1;
	WindowApp window("My Application");
	if (window.Initialize())
	{
		val = window.Run();
	}
	return val;

}