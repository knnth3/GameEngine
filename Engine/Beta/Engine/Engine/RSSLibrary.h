#pragma once
#include "Library.h"
#include "DirectX_RSS.h"


namespace Engine
{
	class RSSLibrary
	{
	public:
		RSSLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize();
		void Clear();
		const std::shared_ptr<DirectX_RSS> GetRSS(const std::string& uniqueName);
		const std::shared_ptr<DirectX_RSS> CreateRSS(const std::string& uniqueName);
		void DeleteRSS(const std::string& uniqueName);
		bool SetAsActive(const std::string& uniqueName);
	private:

		std::string m_activeRSS;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		DirectX_RSS m_default;
		std::map<std::string, int> m_rssCodex;
		Library<DirectX_RSS> m_rss;
	};
}

