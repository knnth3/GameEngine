#pragma once
#include <Net\Server.h>
#include <memory>

namespace Lime
{
	class Server
	{
	public:
		DLL_EXPORT Server();
		DLL_EXPORT ~Server();
		DLL_EXPORT void Initialize();
	private:
		std::unique_ptr<Net::Server> m_Server;
	};
}

