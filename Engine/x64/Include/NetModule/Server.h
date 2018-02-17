#pragma once
#include <chrono>
#include <atomic>
#include <future>
#include <mutex>
#include "NetModule.h"
#include "Basics.h"
#include "Transciever.h"

//Server program and fundamentals
//Author: Eric Marquez
//+Includes the ability to poll connections for various uses
//+Opens new thread that controlls the node for a specific peer
//+Uses map<index, queue> for quick indexing, insert, and extract
//+ID can be obtained from m_connections with username
//Transciever queue

namespace Net
{
	class Server
	{
	public:
		NET_API Server(const std::string& password = "");
		NET_API ~Server();
		NET_API bool Open(uint16_t port);
		NET_API void SetPassword(const std::string& password);
		NET_API bool Send(const uint32_t& address, const std::string& data);
		NET_API bool Send(const uint32_t& address, const ByteBuffer& data);
		NET_API bool SendToAll(const ByteBuffer& data);
		NET_API bool Recieve(const uint32_t& address, ByteBuffer& packet);
		NET_API bool RecieveAll(std::vector<SimplePacket>& recieved);
		NET_API bool GetNewLoginRequest(uint32_t& ID, std::string& username);
		NET_API bool GetLogoutRequest(uint32_t& address);
		NET_API void CloseConnection(uint32_t& address);
		NET_API void CloseAllConnections();

	private:

		bool m_bIsInit;
		std::string m_password;
		std::unique_ptr<Transciever> m_transciever;
	};


}