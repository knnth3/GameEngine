#include <Lime\Primitives.h>
#include <Lime\DX11WindowApp.h>
#include <Lime\DX11Renderer.h>

int main()
{
	using namespace Lime;
	Vertex2 v[] =
	{
		Vertex2(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex2(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex2(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex2(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	Lime::DX11WindowApp window(L"DX11 Render Test");
	Lime::DX11Renderer rend(window.GetHWND(),
		window.GetWindowWidth(), window.GetWindowHeight());
	auto camera = std::make_shared<Lime::DX11Camera>(window.GetWindowWidth(), window.GetWindowHeight());
	rend.AttatchCamera(camera);

	rend.AddModel(v, ARRAYSIZE(v), indices, ARRAYSIZE(indices));
	rend.Initialize();
	while (window.Run())
	{
		rend.Render();
	}
	rend.Close();
	return 0;
}