#pragma once
#include "Library.h"
#include "GraphicsRSS.h"


namespace Graphics
{
	class RSSLibrary
	{
	public:
		RSSLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize();
		void Clear();
		const std::shared_ptr<GraphicsRSS> GetRSS(const std::string& uniqueName);
		const std::shared_ptr<GraphicsRSS> CreateRSS(const std::string& uniqueName);
		void DeleteRSS(const std::string& uniqueName);
		bool SetAsActive(const std::string& uniqueName);
	private:

		std::string m_activeRSS;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::map<std::string, int> m_rssCodex;
		Library<GraphicsRSS> m_rss;
	};
}
