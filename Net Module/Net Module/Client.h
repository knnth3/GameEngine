#pragma once
#include <chrono>
#include <atomic>
#include <future>
#include "Helper.h"
#include "UDPSocket.h"
#include "EPacket.h"
#include "Node.h"
#include "Transciever.h"

class Client
{
public:
	Client(std::string address, unsigned short port);
	void Close();
	bool Initialize();
	void Send(std::string s);
private:
	void Update();
	ConnectionType ConnectClient(std::shared_ptr<Address>& address, ProgramData& data);
	void Login(std::string s);
	uint32_t GenerateKey(uint32_t seed);

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

