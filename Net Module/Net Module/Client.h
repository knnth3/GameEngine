#pragma once
#include <chrono>
#include <atomic>
#include <future>
#include "Helper.h"
#include "UDPSocket.h"
#include "EPacket.h"
#include "Node.h"
#include "Transciever.h"

namespace Net
{
	class Client
	{
	public:
		NET_API Client(std::string address, unsigned short port);
		NET_API void Close();
		NET_API bool Initialize();
		NET_API void Send(std::string s);
		NET_API void Send(edata data);
		NET_API edata Recive();
	private:
		void Update();
		ConnectionType ConnectClient(std::shared_ptr<Address>& address, ProgramData& data);
		void Login(std::string s);
		Identification GenerateKey(Identification seed);

		PDQueue m_Queue;
		TQueue m_sendingDB;
		std::shared_ptr<TDataBase> m_recievedDB;
		AddressPtr m_ServerAddress;
		std::unique_ptr<Node> m_ClientNode;
		std::unique_ptr<Transciever> m_Transciever;

		std::atomic<bool> m_closeThread;
		std::future<void> m_asyncThread;

		bool m_loggedIn;
		bool m_isInit;
	};


}