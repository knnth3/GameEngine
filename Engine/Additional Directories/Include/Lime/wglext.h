#pragma once
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl\GL.h>

namespace Lime
{
	//Functions for Opengl API use
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE0                       0x84C0
#define GL_BGRA                           0x80E1
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

	typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
	typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
	typedef void (APIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
	typedef void (APIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
	typedef void (APIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
	typedef void (APIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage);
	typedef void (APIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
	typedef GLuint(APIENTRY * PFNGLCREATEPROGRAMPROC) (void);
	typedef GLuint(APIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
	typedef void (APIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
	typedef void (APIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
	typedef void (APIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
	typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
	typedef void (APIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
	typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
	typedef void (APIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
	typedef void (APIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
	typedef GLint(APIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const char *name);
	typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
	typedef void (APIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
	typedef void (APIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog);
	typedef void (APIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
	typedef void (APIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
	typedef void (APIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char* *string, const GLint *length);
	typedef void (APIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
	typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	typedef void (APIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const char *name);
	typedef GLint(APIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const char *name);
	typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
	typedef void (APIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
	typedef void (APIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
	typedef void (APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
	typedef void (APIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
	typedef void (APIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);

	static PFNWGLCHOOSEPIXELFORMATARBPROC      WglChoosePixelFormatARB = NULL;
	static PFNWGLCREATECONTEXTATTRIBSARBPROC   WglCreateContextAttribsARB = NULL;
	static PFNWGLSWAPINTERVALEXTPROC           WglSwapIntervalEXT = NULL;
	static PFNGLATTACHSHADERPROC               WglAttachShader = NULL;
	static PFNGLBINDBUFFERPROC                 WglBindBuffer = NULL;
	static PFNGLBINDVERTEXARRAYPROC            WglBindVertexArray = NULL;
	static PFNGLBUFFERDATAPROC                 WglBufferData = NULL;
	static PFNGLCOMPILESHADERPROC              WglCompileShader = NULL;
	static PFNGLCREATEPROGRAMPROC              WglCreateProgram = NULL;
	static PFNGLCREATESHADERPROC               WglCreateShader = NULL;
	static PFNGLDELETEBUFFERSPROC              WglDeleteBuffers = NULL;
	static PFNGLDELETEPROGRAMPROC              WglDeleteProgram = NULL;
	static PFNGLDELETESHADERPROC               WglDeleteShader = NULL;
	static PFNGLDELETEVERTEXARRAYSPROC         WglDeleteVertexArrays = NULL;
	static PFNGLDETACHSHADERPROC               WglDetachShader = NULL;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC    WglEnableVertexAttribArray = NULL;
	static PFNGLGENBUFFERSPROC                 WglGenBuffers = NULL;
	static PFNGLGENVERTEXARRAYSPROC            WglGenVertexArrays = NULL;
	static PFNGLGETATTRIBLOCATIONPROC          WglGetAttribLocation = NULL;
	static PFNGLGETPROGRAMINFOLOGPROC          WglGetProgramInfoLog = NULL;
	static PFNGLGETPROGRAMIVPROC               WglGetProgramiv = NULL;
	static PFNGLGETSHADERINFOLOGPROC           WglGetShaderInfoLog = NULL;
	static PFNGLGETSHADERIVPROC                WglGetShaderiv = NULL;
	static PFNGLLINKPROGRAMPROC                WglLinkProgram = NULL;
	static PFNGLSHADERSOURCEPROC               WglShaderSource = NULL;
	static PFNGLUSEPROGRAMPROC                 WglUseProgram = NULL;
	static PFNGLVERTEXATTRIBPOINTERPROC        WglVertexAttribPointer = NULL;
	static PFNGLBINDATTRIBLOCATIONPROC         WglBindAttribLocation = NULL;
	static PFNGLGETUNIFORMLOCATIONPROC         WglGetUniformLocation = NULL;
	static PFNGLUNIFORMMATRIX4FVPROC           WglUniformMatrix4fv = NULL;
	static PFNGLACTIVETEXTUREPROC              WglActiveTexture = NULL;
	static PFNGLUNIFORM1IPROC                  WglUniform1i = NULL;
	static PFNGLGENERATEMIPMAPPROC             WglGenerateMipmap = NULL;
	static PFNGLDISABLEVERTEXATTRIBARRAYPROC   WglDisableVertexAttribArray = NULL;
	static PFNGLUNIFORM3FVPROC                 WglUniform3fv = NULL;
	static PFNGLUNIFORM4FVPROC                 WglUniform4fv = NULL;

	static bool LOAD_OPENGL_EXTENSION_LIST()
	{
		// Load the OpenGL extensions that this application will be using.
		WglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		if (!WglChoosePixelFormatARB)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (!WglCreateContextAttribsARB)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (!WglSwapIntervalEXT)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		if (!WglAttachShader)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		if (!WglBindBuffer)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		if (!WglBindVertexArray)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		if (!WglBufferData)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		if (!WglCompileShader)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		if (!WglCreateProgram)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		if (!WglCreateShader)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
		if (!WglDeleteBuffers)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
		if (!WglDeleteProgram)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		if (!WglDeleteShader)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
		if (!WglDeleteVertexArrays)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
		if (!WglDetachShader)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		if (!WglEnableVertexAttribArray)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		if (!WglGenBuffers)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
		if (!WglGenVertexArrays)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		if (!WglGetAttribLocation)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		if (!WglGetProgramInfoLog)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		if (!WglGetProgramiv)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		if (!WglGetShaderInfoLog)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		if (!WglGetShaderiv)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		if (!WglLinkProgram)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		if (!WglShaderSource)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		if (!WglUseProgram)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		if (!WglVertexAttribPointer)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
		if (!WglBindAttribLocation)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		if (!WglGetUniformLocation)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		if (!WglUniformMatrix4fv)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		if (!WglActiveTexture)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		if (!WglUniform1i)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		if (!WglGenerateMipmap)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
		if (!WglDisableVertexAttribArray)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
		if (!WglUniform3fv)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		WglUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
		if (!WglUniform4fv)
		{
			MessageBox(0, GetLastErrorAsString().c_str(), L"OpenGL - Error", MB_OK);
			return false;
		}

		return true;
	}


#define glChoosePixelFormatARB WglChoosePixelFormatARB
#define glCreateContextAttribsARB WglCreateContextAttribsARB
#define glSwapIntervalEXT WglSwapIntervalEXT
#define glAttachShader WglAttachShader;
#define glBindBuffer   WglBindBuffer;
#define glBindVertexArray WglBindVertexArray ;
#define glBufferData WglBufferData;
#define glCompileShader WglCompileShader;
#define glCreateProgram WglCreateProgram;
#define glCreateShader WglCreateShader;
#define glDeleteBuffers WglDeleteBuffers;
#define glDeleteProgram WglDeleteProgram;
#define glDeleteShader WglDeleteShader;
#define glDeleteVertexArrays WglDeleteVertexArrays;
#define glDetachShader WglDetachShader;
#define glEnableVertexAttribArray WglEnableVertexAttribArray;
#define glGenBuffers WglGenBuffers;
#define glGenVertexArrays WglGenVertexArrays;
#define glGetAttribLocation WglGetAttribLocation;
#define glGetProgramInfoLog WglGetProgramInfoLog;
#define glGetProgramiv WglGetProgramiv;
#define glGetShaderInfoLog WglGetShaderInfoLog;
#define glGetShaderiv WglGetShaderiv;
#define glLinkProgram WglLinkProgram;
#define glShaderSource WglShaderSource;
#define glUseProgram WglUseProgram;
#define glVertexAttribPointer WglVertexAttribPointer;
#define glBindAttribLocation WglBindAttribLocation;
#define glGetUniformLocation WglGetUniformLocation;
#define glUniformMatrix4fv WglUniformMatrix4fv;
#define glActiveTexture WglActiveTexture;
#define glUniform1i WglUniform1i;
#define glGenerateMipmap WglGenerateMipmap;
#define glDisableVertexAttribArray WglDisableVertexAttribArray;
#define glUniform3fv WglUniform3fv;
#define glUniform4fv WglUniform4fv;

}