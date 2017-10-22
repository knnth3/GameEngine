#include "OpenGLGraphics.h"


Lime::OpenGLGraphics::OpenGLGraphics()
{
	m_deviceContext = 0;
	m_renderingContext = 0;

}

Lime::OpenGLGraphics::~OpenGLGraphics()
{
#if PLATFORM == OS_WINDOWS
	// Release the rendering context.
	if (m_renderingContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_renderingContext);
		m_renderingContext = 0;
	}

	// Release the device context.
	if (m_deviceContext)
	{
		ReleaseDC((HWND)m_window, m_deviceContext);
		m_deviceContext = 0;
	}
#endif
}

bool Lime::OpenGLGraphics::PreInitialize(const void * window)
{
	return InitializeExtensions(window);
}

bool Lime::OpenGLGraphics::Initialize(const void * window, const uint32_t width, const uint32_t height)
{
	m_window = window;
	return InitOpenGL(window);
}

bool Lime::OpenGLGraphics::Add3DModel(std::shared_ptr<Model::Model3D>& model)
{
	return false;
}

bool Lime::OpenGLGraphics::Add2DModel(std::shared_ptr<Model::Model2D>& model)
{
	return false;
}

bool Lime::OpenGLGraphics::Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color)
{
	return false;
}

bool Lime::OpenGLGraphics::AddText(std::string text, std::shared_ptr<TextController>& controller)
{
	return false;
}

void Lime::OpenGLGraphics::AttatchCamera(std::shared_ptr<Camera>& ptr)
{
	
}

void Lime::OpenGLGraphics::Draw()
{
	//TODO: Draw Implementation
	SwapBuffers(m_deviceContext);
}

void Lime::OpenGLGraphics::ResizeWindow(const uint32_t width, const uint32_t height)
{
}

void Lime::OpenGLGraphics::Wireframe(bool statement)
{
	
}

void Lime::OpenGLGraphics::ClearScreen(glm::vec3 color)
{
	// Set the color to clear the screen to.
	glClearColor(color.r, color.g, color.b, 1.0f);

	// Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lime::OpenGLGraphics::Reset()
{
	
}

bool Lime::OpenGLGraphics::InitOpenGL(const void * window)
{
#if PLATFORM == OS_WINDOWS
	int attributeListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int attributeList[5];
	char *vendorString, *rendererString;


	// Get the device context for this window.
	m_deviceContext = GetDC((HWND)window);
	if (!m_deviceContext)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Support for OpenGL rendering.
	attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	// Support for rendering to a window.
	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	// Support for hardware acceleration.
	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	// Support for 24bit color.
	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	// Support for 24 bit depth buffer.
	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = 24;

	// Support for double buffer.
	attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	// Support for swapping front and back buffer.
	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	// Support for the RGBA pixel type.
	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	// Support for a 8 bit stencil buffer.
	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = 8;

	// Null terminate the attribute list.
	attributeListInt[18] = 0;

	// Query for a pixel format that fits the attributes we want.
	result = wglChoosePixelFormatARB(m_deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);
	if (result != 1)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// If the video card/display can handle our desired pixel format then we set it as the current one.
	result = SetPixelFormat(m_deviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if (result != 1)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Set the 4.0 version of OpenGL in the attribute list.
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	// Null terminate the attribute list.
	attributeList[4] = 0;

	// Create a OpenGL 4.0 rendering context.
	m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attributeList);
	if (m_renderingContext == NULL)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Set the rendering context to active.
	result = wglMakeCurrent(m_deviceContext, m_renderingContext);
	if (result != 1)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Set the depth buffer to be entirely cleared to 1.0 values.
	glClearDepth(1.0f);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Set the polygon winding to front facing for the left handed system.
	glFrontFace(GL_CW);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Get the name of the video card.
	vendorString = (char*)glGetString(GL_VENDOR);
	rendererString = (char*)glGetString(GL_RENDERER);

	// Store the video card name in a class member variable so it can be retrieved later.
	strcpy_s(m_videoCardDescription, vendorString);
	strcat_s(m_videoCardDescription, " - ");
	strcat_s(m_videoCardDescription, rendererString);

	// Turn on or off the vertical sync depending on the input bool value.
	//if (vsync)
	//{
	//	result = wglSwapIntervalEXT(1);
	//}
	//else
	//{
	//	result = wglSwapIntervalEXT(0);
	//}

	//// Check if vsync was set correctly.
	//if (result != 1)
	//{
	//	return false;
	//}

	return true;

#endif
}

bool Lime::OpenGLGraphics::LoadExtensionList()
{
	// Load the OpenGL extensions that this application will be using.
#if PLATFORM == OS_WINDOWS

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!wglSwapIntervalEXT)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	if (!glAttachShader)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if (!glBindBuffer)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	if (!glBindVertexArray)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	if (!glBufferData)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	if (!glCompileShader)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	if (!glCreateProgram)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	if (!glCreateShader)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	if (!glDeleteBuffers)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	if (!glDeleteProgram)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	if (!glDeleteShader)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	if (!glDeleteVertexArrays)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	if (!glDetachShader)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if (!glEnableVertexAttribArray)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if (!glGenBuffers)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	if (!glGenVertexArrays)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	if (!glGetAttribLocation)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	if (!glGetProgramInfoLog)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	if (!glGetProgramiv)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	if (!glGetShaderInfoLog)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	if (!glGetShaderiv)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	if (!glLinkProgram)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	if (!glShaderSource)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	if (!glUseProgram)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if (!glVertexAttribPointer)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	if (!glBindAttribLocation)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	if (!glGetUniformLocation)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	if (!glUniformMatrix4fv)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	if (!glActiveTexture)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	if (!glUniform1i)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	if (!glGenerateMipmap)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	if (!glDisableVertexAttribArray)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	if (!glUniform3fv)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	if (!glUniform4fv)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

#endif

	return true;
}

bool Lime::OpenGLGraphics::InitializeExtensions(const void * window)
{
#if PLATFORM == OS_WINDOWS

	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;


	// Get the device context for this window.
	deviceContext = GetDC((HWND)window);
	if (!deviceContext)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Set a temporary default pixel format.
	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if (error != 1)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Create a temporary rendering context.
	renderContext = wglCreateContext(deviceContext);
	if (!renderContext)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Set the temporary rendering context as the current rendering context for this window.
	error = wglMakeCurrent(deviceContext, renderContext);
	if (error != 1)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Initialize the OpenGL extensions needed for this application.  Note that a temporary rendering context was needed to do so.
	result = LoadExtensionList();
	if (!result)
	{
		MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
		return false;
	}

	// Release the temporary rendering context now that the extensions have been loaded.
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = NULL;

	// Release the device context for this window.
	ReleaseDC((HWND)window, deviceContext);
	deviceContext = 0;

	return result;
#else
	//TODO: add for OS X and Linux 
#endif
}
