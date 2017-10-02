#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <comdef.h>
#include <map>
#include "Camera.h"
#include "TextController.h"
#include "Primitives.h"
#include "D3Dcompiler.h"

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

namespace Lime
{
	struct MatrixBuffer
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct TextBuffer
	{
		glm::vec4 PosAscii;
	};
	struct PF_PixelBuffer
	{
		glm::vec4 colorBlend;
		glm::vec4 diffuseColor;
		glm::vec3 lightDirection;
		float padding;
	};

	class DX11Graphics
	{
	public:
		DLL_EXPORT DX11Graphics(const HWND window, const UINT width, const UINT height);
		DLL_EXPORT ~DX11Graphics();
		DLL_EXPORT void DrawModel(std::shared_ptr<Model3D>& model);
		DLL_EXPORT void DrawText(std::string text , std::shared_ptr<TextController>& controller);
		DLL_EXPORT void AttatchCamera(std::shared_ptr<Camera>& ptr);
		DLL_EXPORT Texture LoadTextureFromFile(std::wstring filename);
		DLL_EXPORT HRESULT CreateShaders(LPCWSTR vsPath, LPCWSTR psPath, D3D11_INPUT_ELEMENT_DESC* layout, size_t layoutSize);
		DLL_EXPORT void Draw();
		DLL_EXPORT ID3D11DeviceContext* GetDeviceContext() const;
		DLL_EXPORT ID3D11RenderTargetView* GetRenderTargetView() const;
		DLL_EXPORT ID3D11DepthStencilView* GetDepthStencilView() const;
		DLL_EXPORT D3D11_VIEWPORT GetScreenViewport() const;
		DLL_EXPORT void ResizeWindow(const UINT width, const UINT height);
		DLL_EXPORT void Wireframe(bool statement);
		DLL_EXPORT void Reset();
	private:

		void Close();
		void RenderText(std::string text, std::shared_ptr<Model3D> model);
		void RenderMesh(std::shared_ptr<Model3D> model);
		HRESULT Initialize(const HWND window, const UINT width, const UINT height);
		HRESULT CreateBuffers();
		HRESULT CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateDepthStencil(const UINT width, const UINT height);
		HRESULT CreateBlendState();
		HRESULT CreateRTV();
		void CreateViewport(const UINT width, const UINT height);
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, 
			LPCSTR profile, ID3DBlob** blob);


		bool m_hasBuffers;
		HINSTANCE m_hInstance;
		IDXGISwapChain* SwapChain;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Buffer* m_ObjConstBuffer;
		ID3D11Buffer* m_transparentBuffer;
		ID3D11Buffer* m_textBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11Buffer* m_vertexBuffer;
		std::vector<ID3D11ShaderResourceView*> m_textures;
		std::vector<ID3D11SamplerState*> m_samplerStates;
		std::vector<ID3D11VertexShader*> m_vertexShaders;
		std::vector<ID3D11InputLayout*> m_vertLayouts;
		std::vector<ID3D11PixelShader*> m_pixelShaders;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11Texture2D* depthStencilBuffer;
		std::shared_ptr<Camera> m_camera;
		ID3D11RasterizerState* WireFrame;
		ID3D11Device* m_dx11device;
		ID3D11DeviceContext* m_dx11Context;
		D3D11_VIEWPORT viewport;
		std::vector<std::shared_ptr<Model3D>> m_models;
		ModelData m_modelLib;
		std::vector<UINT> cachedIDs;
		bool hasConsBuffers = false;
		ID3D11BlendState* Transparency;
		ID3D11BlendState* TransparencyBack;
		ID3D11RasterizerState* CCWcullMode;
		ID3D11RasterizerState* CWcullMode;
		DiffuseLight m_sun;
		UINT m_bufferCount;
		bool m_isWireframe = false;
	};


}