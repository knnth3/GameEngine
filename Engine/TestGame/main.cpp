#include <Lime\DX11WindowApp.h>
#include <Lime\DX11Renderer.h>
#include <chrono>

int main()
{
	auto start = std::chrono::system_clock::now();
	auto end = start;
	using namespace Lime;
	float rot = 0.01f;
	auto model = std::make_shared<Model2>();
	model->m_Data->m_Verticies =
	{
		// Front Face
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			  						 		
		// Back Face						
		Vertex2(-1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
			  								
		// Top Face							
		Vertex2(-1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f, 1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f, 1.0f,  1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			  								
		// Bottom Face						
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex2(1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
			  									
		// Left Face							
		Vertex2(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			  									
		// Right Face							
		Vertex2(1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex2(1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex2(1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
	};
	model->m_Data->m_Indicies = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};


	Lime::DX11WindowApp window(L"DX11 Render Test");
	Lime::DX11Renderer renderer(window.GetHWND(),
		window.Width(), window.Height());
	auto camera = std::make_shared<Lime::DX11Camera>(window.Width(), window.Height());
	renderer.AttatchCamera(camera);
	model->Color(1.0f, 1.0f, 1.0f);
	renderer.AddModel(model);
	renderer.LoadShaderFromFile(L"images.dds");
	model->Translate(3.0f,0.0f,-3.0f);
	while (window.Run())
	{
		end = std::chrono::system_clock::now();
		std::chrono::duration<float> time = end - start;
		//Keep the cubes rotating
		float seconds = time.count(); 
		rot += 9.8/2000.0f;
		if (rot > 6.28f)
			rot = 0.0f;
		model->Rotate(rot, rot, rot);
		model->RotateAtOrigin(rot, rot, rot);
		renderer.Draw();
		start = end;
	}
	return 0;
}