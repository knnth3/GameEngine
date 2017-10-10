#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <comdef.h>
#include <map>
#include "Camera.h"
#include "TextController.h"
#include "Shared.h"
#include "D3Dcompiler.h"
#include "DX11Texture.h"
#include "DX11Shader.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"

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
		glm::vec4 color;
		glm::vec3 cameraPos;
		float padding;
	};

	struct TextBuffer
	{
		glm::vec4 PosAscii;
		glm::vec4 color;
	};
	struct PF_PixelBuffer
	{
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec3 lightDirection;
		float specularPower;
	};

	class DX11Graphics
	{
	public:
		AppDLL_API DX11Graphics(const HWND window, const UINT width, const UINT height);
		AppDLL_API ~DX11Graphics();
		AppDLL_API bool AddModel(std::shared_ptr<Model::Model3D>& model);
		AppDLL_API bool AddText(std::string text, std::shared_ptr<TextController>& controller);
		AppDLL_API void AttatchCamera(std::shared_ptr<Camera>& ptr);
		AppDLL_API void Draw();
		AppDLL_API void ResizeWindow(const UINT width, const UINT height);
		AppDLL_API void Wireframe(bool statement);
		AppDLL_API void ClearScreen(glm::vec3 color);
		AppDLL_API void Reset();
	private:

		void Close();
		void RenderText(std::string text, std::shared_ptr<Model::Model3D>& model);
		void RenderMesh(std::shared_ptr<Model::Model3D>& model);
		void Render2DMesh(std::shared_ptr<Model::Model3D>& model);
		HRESULT Initialize(const HWND window, const UINT width, const UINT height);
		void CreateBuffers();
		void CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateBlendState();
		HRESULT CreateRTV();
		void CreateViewport(const UINT width, const UINT height);
		void SetZBufferStatus(const bool value);


		ID3D11RasterizerState* WireFrame;
		ID3D11RasterizerState* m_cullBack;
		bool m_isWireframe = false;

		//New API
		const UINT m_bufferCount;
		WorldLight m_light;
		int m_windowWidth;
		int m_windowHeight;
		std::shared_ptr<Camera> m_camera;
		Model::VertexLibrary m_newModelLib;
		HINSTANCE m_hInstance;
		D3D11_VIEWPORT m_viewport;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11BlendState* Transparency;
		ID3D11BlendState* TransparencyBack;

		//Buffers
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;
		std::unique_ptr<DX11BufferManager> m_bufferManager;

		//Dpeth Stencil state
		std::unique_ptr<DX11DepthStencilState> m_dsState;

		//Shaders
		std::unique_ptr<DX11Shader> m_3DmodelShader;
		std::unique_ptr<DX11Shader> m_2DmodelShader;
		std::unique_ptr<DX11Shader> m_textShader;

		//Textures
		std::unique_ptr<DX11Texture> m_modelTexture;
		std::unique_ptr<DX11Texture> m_2DmodelTexture;
		std::unique_ptr<DX11Texture> m_textTexture;

	};


}