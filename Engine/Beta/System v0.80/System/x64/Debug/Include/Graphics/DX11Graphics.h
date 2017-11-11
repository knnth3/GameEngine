#pragma once
#include "RenderAPI.h"
#include "DllSettings.h"
#include "DX11Shader.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"
#include "VertexManager.h"

namespace Graphics
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

	struct Float8
	{
		glm::vec4 one;
		glm::vec4 two;
	};
	struct PF_PixelBuffer
	{
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec3 lightDirection;
		float specularPower;
	};

	class DX11Graphics:
		public RenderAPI
	{
	public:
		DX11Graphics();
		~DX11Graphics();
		void ResizeWindow(const uint32_t width, const uint32_t height);
		void Wireframe(bool statement);
		void Reset();


		//Overloaded Functions;
		virtual bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen) override;
		virtual void BeginScene() override;
		virtual void EndScene() override;
		virtual void Close() override;
		virtual void GetVideoCardInfo(std::string& name, int& memory) override;
		virtual void ClearScreen(float r, float g, float b) override;
		virtual void AddModel(std::shared_ptr<Model::Model3D>& model) override;
		virtual void AttatchCamera(std::shared_ptr<Camera>& ptr) override;

	private:
		void Draw();
		void Graphics::DX11Graphics::ProcessObject_3D(VertexManagerInfo & info);
		void FillBuffers();
		void CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateBlendState();
		HRESULT CreateRTV();
		void CreateViewport(const uint32_t width, const uint32_t height);
		void SetZBufferStatus(const bool value);

		ID3D11RasterizerState* WireFrame;
		ID3D11RasterizerState* m_cullBack;
		bool m_isWireframe = false;
		bool m_bFilled = false;

		//New API
		int m_videoCardMemory;
		std::string m_videoCardDescription;
		const uint32_t m_bufferCount;
		WorldLight m_light;
		int m_windowWidth;
		int m_windowHeight;
		std::shared_ptr<Camera> m_camera;
		HINSTANCE m_hInstance;
		D3D11_VIEWPORT m_viewport;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11BlendState* Transparency;
		ID3D11BlendState* TransparencyBack;

		//Buffers
		std::shared_ptr<VertexManager> m_vertexManager;
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;
		std::unique_ptr<DX11BufferManager> m_bufferManager;

		//Dpeth Stencil state
		std::unique_ptr<DX11DepthStencilState> m_depthStencilState;

		//Shaders
		std::unique_ptr<DX11Shader> m_3DmodelShader;
		std::unique_ptr<DX11Shader> m_2DmodelShader;
		std::unique_ptr<DX11Shader> m_textShader;
		std::unique_ptr<DX11Shader> m_lineShader;

		//3D Processing
		std::vector<int> m_modelDrawCache_3D;
		std::map<int, VertexManagerInfo> m_cachedModels_3D;

	};


}