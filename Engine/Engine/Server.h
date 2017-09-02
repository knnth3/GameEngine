#pragma once
#include <Net\Server.h>
#include <memory>

namespace Lime
{
	class Server
	{
	public:
		DLL_EXPORT Server(short Port);
		DLL_EXPORT void Initialize();
		DLL_EXPORT void Send(int ID, std::vector<byte>& data);
		DLL_EXPORT bool Recieve(int ID, std::vector<byte>& data);
		DLL_EXPORT uint16_t GetID(std::string username);
		DLL_EXPORT void GetConnectedUsers(std::vector<std::string>& usernames);
		DLL_EXPORT void Close();
	private:
		std::unique_ptr<Net::Server> m_Server;
	};
}

