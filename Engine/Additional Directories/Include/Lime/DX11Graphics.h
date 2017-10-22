#pragma once
#include "RenderBatch.h"

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

	class DX11Graphics :
		public RenderBatch
	{
	public:
		AppDLL_API DX11Graphics();
		AppDLL_API ~DX11Graphics();
		AppDLL_API virtual bool Initialize(const void* window, const uint32_t width, const uint32_t height) override;
		AppDLL_API virtual bool Add3DModel(std::shared_ptr<Model::Model3D>& model) override;
		AppDLL_API virtual bool Add2DModel(std::shared_ptr<Model::Model2D>& model) override;
		AppDLL_API virtual bool Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color) override;
		AppDLL_API virtual bool AddText(std::string text, std::shared_ptr<TextController>& controller) override;
		AppDLL_API virtual void AttatchCamera(std::shared_ptr<Camera>& ptr) override;
		AppDLL_API virtual void Draw() override;
		AppDLL_API virtual void ResizeWindow(const uint32_t width, const uint32_t height) override;
		AppDLL_API virtual void Wireframe(bool statement) override;
		AppDLL_API virtual void ClearScreen(glm::vec3 color) override;
		AppDLL_API virtual void Reset() override;
	private:

		void Close();
		void RenderText(std::string text, std::shared_ptr<Model::Model3D>& model);
		void RenderMesh(std::shared_ptr<Model::Model3D>& model);
		void Render2DMesh(std::shared_ptr<Model::Model2D>& model);
		void RenderLine(std::shared_ptr<Model::Model3D>& model);
		void CreateBuffers();
		void CreateConstBuffers();
		HRESULT CreateRenderStates();
		HRESULT CreateBlendState();
		HRESULT CreateRTV();
		void CreateViewport(const uint32_t width, const uint32_t height);
		void SetZBufferStatus(const bool value);
		void ResetView();

		ID3D11RasterizerState* WireFrame;
		ID3D11RasterizerState* m_cullBack;
		bool m_isWireframe = false;
		bool m_hasCreatedBuffers = false;

		//New API
		const uint32_t m_bufferCount;
		WorldLight m_light;
		int m_windowWidth;
		int m_windowHeight;
		std::shared_ptr<Camera> m_camera;
		Model::VertexLibrary m_modelLib;
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
		std::unique_ptr<DX11Shader> m_lineShader;

	};


}