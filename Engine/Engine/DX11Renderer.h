#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <comdef.h>
#include "DX11Camera.h"
#include "Primitives.h"
#include "D3Dcompiler.h"

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

namespace Lime
{
	class DX11Renderer
	{
	public:
		DLL_EXPORT DX11Renderer(HWND window, int width, int height);
		DLL_EXPORT ~DX11Renderer();
		DLL_EXPORT void AddModel(std::shared_ptr<Model2>& model);
		DLL_EXPORT void AttatchCamera(std::shared_ptr<DX11Camera>& ptr);
		DLL_EXPORT void LoadShaderFromFile(std::wstring filename);
		DLL_EXPORT void Draw();
		void Close();
		HRESULT Initialize();
		HRESULT CreateBuffers();
		HRESULT CreateShaders();
		HRESULT CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateDepthStencil();
		void CreateViewport();
		XMMATRIX GlmToXM(glm::mat4 matrix);
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, 
			LPCSTR profile, ID3DBlob** blob);
		bool m_hasBuffers;
		std::vector<glm::vec3> m_verticies;
		unsigned int m_width, m_height;
		HWND m_window;
		HINSTANCE m_hInstance;
		IDXGISwapChain* SwapChain;
		ID3D11Device* m_dx11device;
		ID3D11DeviceContext* m_dx11Context;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Buffer* m_ObjConstBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11Buffer* m_vertexBuffer;
		LPCWSTR m_psPath;
		LPCWSTR m_vsPath;
		ID3DBlob *vsBlob = nullptr;
		ID3DBlob *psBlob = nullptr;
		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;
		ID3D11InputLayout* vertLayout;
		std::vector<ID3D11ShaderResourceView*> m_textures;
		std::vector<ID3D11SamplerState*> m_samplerStates;
		std::vector<ID3D11VertexShader*> m_vertexShaders;
		std::vector<ID3D11PixelShader*> m_pixelShaders;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;
		std::shared_ptr<DX11Camera> m_camera;
		ID3D11RasterizerState* WireFrame;
		struct ConstBuffer
		{
			XMMATRIX WVP;
		};
		ConstBuffer m_ObjBuffer;
		std::vector<std::shared_ptr<Model2>> m_models;

	};


}