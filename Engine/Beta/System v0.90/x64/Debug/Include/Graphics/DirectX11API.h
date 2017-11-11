#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "RenderAPI.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"
#include "VertexManager.h"
#include "DX11ShaderManager.h"

namespace Graphics
{
	class DirectX11API :
		public RenderAPI
	{
	public:
		DirectX11API();
		~DirectX11API();

		//Overloaded Functions
		virtual bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen) override;
		virtual void BeginScene() override;
		virtual void EndScene() override;
		virtual void Close() override;
		virtual void GetVideoCardInfo(std::string& name, int& memory)override;
		virtual void ClearScreen(float r, float g, float b) override;
		virtual std::shared_ptr<DX11BufferManager> GetBufferManager() override;
		virtual std::shared_ptr<DX11ShaderManager> GetShaderManager() override;
		virtual std::shared_ptr<DX11DepthStencilState> GetDepthStencilState() override;

	private:
		HRESULT CreateRSS();
		HRESULT CreateRTV();
		bool CreateDepthStenciState(int screenWidth, int screenHeight);
		void CreateBufferManager();
		void CreateShaderManager();
		void SetViewPort(int screenWidth, int screenHeight);
		HRESULT CreateBlendState();

		bool m_vsync_enabled;
		int m_videoCardMemory;
		std::string m_videoCardDescription;
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11RasterizerState* m_rasterState;
		ID3D11BlendState* Transparency;
		std::shared_ptr<DX11DepthStencilState> m_depthStencilState;
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;
		std::shared_ptr<DX11BufferManager> m_bufferManager;
		std::shared_ptr<DX11ShaderManager> m_shaderManager;
	};

}
