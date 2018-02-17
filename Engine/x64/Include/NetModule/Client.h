#pragma once
#include <chrono>
#include <atomic>
#include <future>
#include "NetModule.h"
#include "Node.h"
#include "Transciever.h"

namespace Net
{
	//Time is represented in ms
	//For Login only, the characters ':', '-', and ' ' are reserved and function will return false if used.
	class Client
	{
	public:
		NET_API Client();
		NET_API ~Client();
		NET_API bool Initialize(const std::string& address, uint16_t port);
		NET_API bool Login(const std::string& username, const std::string& password = "", double timeout = 5000);
		NET_API void Logout();
		NET_API bool Send(const std::string& data);
		NET_API bool Send(const ByteBuffer& data);
		NET_API bool Recieve(ByteBuffer& data);
		NET_API bool GetLogoutRequest(uint32_t& address);

	private:

		bool IsStringValid(const std::string& value);

		bool m_bIsInit;
		Address m_address;
		std::unique_ptr<Transciever> m_transciever;
	};


}