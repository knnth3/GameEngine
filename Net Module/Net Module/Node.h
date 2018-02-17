#pragma once
#include "Basics.h"
#include <mutex>

//Connection Node and ackdata
//Author: Eric Marquez
//+Keeps track of lost packages by recording recieved ones in a bitfield representing
//up to 32 packets behind.
//+Once a packet is sent back, the most up to date version of the bitfield will also go.

namespace Net
{

	typedef std::queue<std::shared_ptr<NetPacket>> PacketQueue;

	class Node
	{
	public:
		Node(const Address& address, const std::string& name);
		void AddSendPacket(const std::shared_ptr<NetPacket>& data);
		void AddRecievedPacket(const std::shared_ptr<NetPacket>& data);

		bool GetNextSendingPacket(std::shared_ptr<NetPacket>& data);
		const std::string& GetName()const;
		const Address& GetAddress()const;
		bool GetNextPacket(ByteBuffer& data);
		bool SetNextPacket(const ByteBuffer& data);

	private:

		std::string m_name;
		std::mutex m_lock;
		Address m_address;
		PacketQueue m_recieved;
		PacketQueue m_sending;
		uint32_t m_RemoteAckData;
		uint32_t m_LocalCSP;
		uint32_t m_RemoteCSP;
	};


}