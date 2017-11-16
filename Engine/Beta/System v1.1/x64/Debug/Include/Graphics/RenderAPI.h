#pragma once
#include "DllSettings.h"
#include "DX11BufferManager.h"
#include "DX11ShaderManager.h"
#include "DX11DepthStencilState.h"
#include "DX11RasterStateManager.h"


namespace Graphics
{
	class RenderAPI
	{
	public:
		virtual bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen) = 0;
		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;
		virtual void Close() = 0;
		virtual void GetVideoCardInfo(std::string& name, int& memory) = 0;
		virtual void ClearScreen(float r, float g, float b) = 0;
		virtual void SetWindowSize(uint16_t  width, uint16_t  height) = 0;
		virtual std::shared_ptr<DX11BufferManager> GetBufferManager() = 0;
		virtual std::shared_ptr<DX11ShaderManager> GetShaderManager() = 0;
		virtual std::shared_ptr<DX11DepthStencilState> GetDepthStencilState() = 0;
		virtual std::shared_ptr<DX11RasterStateManager> GetRasterStateManager() = 0;
	};
}


