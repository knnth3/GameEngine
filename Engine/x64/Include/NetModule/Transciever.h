#pragma once
#include "UDPSocket.h"
#include "Basics.h"
#include "Node.h"
#include <map>
#include <functional>
#include <future>

//Author:Eric Marquez
//+Transciever sends and recieves packets from the socket
//+Packets are sent to separated queues to be handled by Nodes
//+Outgoing packets are put into a single queue that the transciever sends out


namespace Net
{
	typedef std::unique_ptr<UDPSocket> Socket;

	struct SimplePacket
	{
		uint32_t Address;
		uint16_t Port;
		ByteBuffer Buffer;
	};

	struct LoginPacket
	{
		uint32_t Address;
		uint16_t Port;
		std::string Username;
		std::string Password;
		std::string Answer;
	};

	typedef std::queue<SimplePacket> SimplePacketQueue;

	class Transciever
	{
	public:
		//Simple interface where user only needs to deal with the update/close function.
		//+SetConnectionFunc must be called(unless another way of opening a Node is available)
		Transciever();
		bool Initialize(unsigned short port, bool isClient = false);
		void Send(const uint32_t& address, const ByteBuffer& buffer);
		void SendToAll(const ByteBuffer& buffer);
		bool Recieve(const uint32_t& address, ByteBuffer& buffer);
		bool RecieveAll(std::vector<SimplePacket>& buffers);
		void SendLoginRequest(const LoginPacket& packet);
		bool GetLoginRequest(LoginPacket& packet);
		void SendLogoutRequest(const uint32_t& address);
		void SendLogoutRequestToAll();
		bool GetLogoutRequest(uint32_t& address);
		bool CreateNode(const uint32_t& address, const uint16_t port, const std::string& name);
		void Close();

	private:

		void Thread_Update_Func();
		void RecievePacket();
		void SendPackets();
		void PlacePacket(std::shared_ptr<NetPacket>& packet);

		bool m_bInit;
		Socket m_socket;
		std::mutex m_lock;
		SimplePacketQueue m_recvdLoginReqs;
		SimplePacketQueue m_sendLoginReqs;
		std::future<void> m_asyncThread;
		std::atomic<bool> m_closeThread;
		std::queue<uint32_t> m_logoutRequests;
		std::map<uint32_t, std::unique_ptr<Node>> m_activeNodes;
	};
}