#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <comdef.h>
#include <map>
#include "Camera.h"
#include "TextController.h"
#include "Lime.h"
#include "D3Dcompiler.h"
#include "DX11Texture.h"
#include "DX11Shader.h"
#include "DX11DepthStencilState.h"

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

namespace Lime
{
	struct vertexInfo
	{
		bool empty();

		std::vector<Model::Vertex> vertices;
		std::vector<uint32_t> indices;
	};
	struct MatrixBuffer
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 cameraPos;
		float padding;
	};

	struct TextBuffer
	{
		glm::vec4 PosAscii;
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
		void RenderText(std::string text, std::shared_ptr<Model::Model3D> model);
		void RenderMesh(std::shared_ptr<Model::Model3D> model);
		HRESULT Initialize(const HWND window, const UINT width, const UINT height);
		HRESULT CreateBuffers();
		HRESULT CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateBlendState();
		HRESULT CreateRTV();
		void CreateViewport(const UINT width, const UINT height);



		ID3D11Buffer* m_ObjConstBuffer;
		ID3D11Buffer* m_transparentBuffer;
		ID3D11Buffer* m_textBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11RasterizerState* WireFrame;
		ID3D11RasterizerState* CCWcullMode;
		ID3D11RasterizerState* CWcullMode;
		ID3D11RasterizerState* NoCull;
		bool m_isWireframe = false;

		//New API
		const UINT m_bufferCount;
		WorldLight m_light;
		int m_windowWidth;
		int m_windowHeight;
		bool hasConsBuffers = false;
		std::shared_ptr<Camera> m_camera;
		Model::VertexBuffer m_newModelLib;
		HINSTANCE m_hInstance;
		D3D11_VIEWPORT m_viewport;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain* SwapChain;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11BlendState* Transparency;
		ID3D11BlendState* TransparencyBack;

		//Dpeth Stencil state
		std::unique_ptr<DX11DepthStencilState> m_newDSState;

		//Shaders
		std::unique_ptr<DX11Shader> m_newModelShader;
		std::unique_ptr<DX11Shader> m_newTextShader;

		//Textures
		std::unique_ptr<DX11Texture> m_newModelTexture;
		std::unique_ptr<DX11Texture> m_newTextTexture;
	};


}