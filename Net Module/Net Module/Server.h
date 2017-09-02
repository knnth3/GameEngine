#pragma once
#include <chrono>
#include <atomic>
#include <future>
#include "NetModule.h"
#include "Basics.h"
#include "Address.h"
#include "IPAuthenticator.h"
#include "Transciever.h"
#include "Node.h"

//Server program and fundamentals
//Author: Eric Marquez
//+Includes the ability to poll connections for various uses
//+Opens new thread that controlls the node for a specific peer
//+Uses map<index, queue> for quick indexing, insert, and extract
//+ID can be obtained from m_connections with username
//Transciever queue

namespace Net
{
	typedef std::map<Identification, PDQueue> DataBase;

	class Server
	{
	public:
		NET_API Server(int port);
		NET_API bool Initialize();
		NET_API Identification GetID(std::string username);
		NET_API void Send(Identification ID, std::vector<byte> data);
		NET_API std::vector<byte> Recieve(Identification ID);
		NET_API void GetConnectedUsers(std::vector<std::string>& usernames);
		NET_API void Close();

	private:
		void Update();
		void CreateNode(std::shared_ptr<Address>& address);
		//Function used to Connect peer(Completely customizable)
		//although nothing will be sent/recieved if a node is not created.
		ConnectionType ConnectPeer(std::shared_ptr<Address>& address, ProgramData& packet);
		Identification GenerateKey(Identification seed);
		bool IsValid(Identification ID);
		bool IsAddressQueried(std::shared_ptr<Address>& address);

		std::vector<std::unique_ptr<Node>> m_ActiveNodes;
		std::unique_ptr<Transciever> m_transciever;
		std::unique_ptr<IPAuthenticator> m_ConnectionTimers;

		// IDs of all connected mapped to username;
		std::map<std::string, std::shared_ptr<Address>> m_connections;

		//Stores data sent from the program that is to be sent to given peer
		std::shared_ptr<DataBase> m_database;

		//Stores packets that have been sent/recieved
		std::shared_ptr<TDataBase> m_recievedDB;
		TQueue m_sendingQueue;

		std::atomic<bool> m_closeThread;
		std::future<void> m_asyncThread;

		bool m_isInit;
	};


}