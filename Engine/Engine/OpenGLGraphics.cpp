#include "OpenGLGraphics.h"
#include <gl\GL.h>
#include "wglext.h"


Lime::OpenGLGraphics::OpenGLGraphics(const void* window, const uint32_t width, const uint32_t height)
{
	//PIXELFORMATDESCRIPTOR_1 a;
	//a.size = sizeof(PIXELFORMATDESCRIPTOR_1);
	//a.version = 1;
	////Format for opengl
	//a.formatSupport = 36;
	//a.typeRGBA = 0;
	//a.bits = 32;
	//a.sixteenBitBuffer = 16;
	//a.mainPlane = 0;
	//PIXELFORMATDESCRIPTOR b;
	//b.dwFlags = 36;
	//b.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	//b.nVersion = 1;
	//b.iPixelType = 0;
	//b.cAccumBits = 32;
	//b.


	//uint32_t pixelFormatTemp = ChoosePixelFormat((HDC)window, (PIXELFORMATDESCRIPTOR)a);

}

Lime::OpenGLGraphics::~OpenGLGraphics()
{
}

bool Lime::OpenGLGraphics::Add3DModel(std::shared_ptr<Model::Model3D>& model)
{
	return bool();
}

bool Lime::OpenGLGraphics::Add2DModel(std::shared_ptr<Model::Model2D>& model)
{
	return bool();
}

bool Lime::OpenGLGraphics::Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color)
{
	return bool();
}

bool Lime::OpenGLGraphics::AddText(std::string text, std::shared_ptr<TextController>& controller)
{
	return bool();
}

void Lime::OpenGLGraphics::AttatchCamera(std::shared_ptr<Camera>& ptr)
{
	
}

void Lime::OpenGLGraphics::Draw()
{
	
}

void Lime::OpenGLGraphics::ResizeWindow(const uint32_t width, const uint32_t height)
{
	
}

void Lime::OpenGLGraphics::Wireframe(bool statement)
{
	
}

void Lime::OpenGLGraphics::ClearScreen(glm::vec3 color)
{
	
}

void Lime::OpenGLGraphics::Reset()
{
	
}
