#pragma once
#include "DllSettings.h"
#include <d3d11.h>

namespace Graphics
{
	struct RSSSettings
	{
		bool AntialiasedLineEnable;
		int CullMode;
		int DepthBias;
		float DepthBiasClamp;
		bool DepthClipEnable;
		int FillMode;
		bool FrontCounterClockwise;
		bool MultisampleEnable;
		bool ScissorEnable;
		float SlopeScaledDepthBias;
	};


	class DX11RasterStateManager
	{
	public:
		DX11RasterStateManager(ID3D11Device* device, ID3D11DeviceContext* context);
		~DX11RasterStateManager();

		bool CreateRSS(const RSSSettings settings, std::string uniqueName);
		bool SetActive(std::string uniqueName);
		bool RemoveRSS(std::string uniqueName);

	private:

		std::string m_active;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		std::map<std::string, ID3D11RasterizerState*> m_RSStates;
	};

}
