#pragma once
#include "UDPSocket.h"
#include "EPacket.h"
#include "Basics.h"
#include <map>
#include <functional>

//Author:Eric Marquez
//+Transciever sends and recieves packets from the socket
//+Packets are sent to separated queues to be handled by Nodes
//+Outgoing packets are put into a single queue that the transciever sends out


namespace Net
{

	enum class ConnectionType
	{
		NewConnect,
		Success,
		Failed
	};

	class Server;

	typedef std::unique_ptr<UDPSocket> Socket;
	typedef std::shared_ptr<std::queue<EPacket>> TQueue;
	typedef std::map<uint32_t, TQueue> TDataBase;
	typedef std::function<ConnectionType(std::shared_ptr<Address>&, ProgramData)> AddFunc;

	class Transciever
	{
	public:
		//Simple interface where user only needs to deal with the update/close function.
		//+SetConnectionFunc must be called(unless another way of opening a Node is available)
		DLL_EXPORT Transciever(unsigned short port);
		DLL_EXPORT bool Init(std::shared_ptr<TDataBase>& recievedDB, TQueue sending);
		//Has to return an integer based off of result. Everything else will be ignored.
		// 0 = failed
		// 1 = success (new)
		// 2 = success (established)
		DLL_EXPORT void SetConnectionFunc(AddFunc connectionFunc);
		DLL_EXPORT void Update();
		DLL_EXPORT void Close();

	private:
		void RecievePacket();
		void SendPacket();

		bool isValid();
		std::shared_ptr<EPacket> GetPacket();
		void PlacePacket(std::shared_ptr<Address>& addr, EPacket& packet);
		ConnectionType ConnectPeer(std::shared_ptr<Address>& addr, ProgramData& packet);

		unsigned short m_port;
		//Completely cusomizable "user connected" function
		AddFunc m_ConnectionFunc;
		Socket m_socket;
		std::shared_ptr<TDataBase> m_recievedDB;
		TQueue m_sendingDB;
	};


}