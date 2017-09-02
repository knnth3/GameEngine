#pragma once
#include <bitset>
#include "EPacket.h"
#include "Address.h"
#include "UDPSocket.h"
#include "Basics.h"

//Connection Node and ackdata
//Author: Eric Marquez
//+Built as an interpretation off from Glenn Fiedler's documentation
//+Node is used to send and recieve packets from client/server.
//+Keeps track of lost packages by recording recieved ones in a bitfield representing
//up to 32 packets behind.
//+Once a packet is sent back, the most up to date version of the bitfield will also go.

namespace Net
{

	struct AckData
	{
		//Most recent package recieved
		uint32_t Acknowledged = 0;

		//Previous packages recieved up to 32 behind
		uint32_t bitfield = 0;
	};

	typedef unsigned char byte;
	typedef std::vector<byte> edata;
	typedef std::shared_ptr<edata> ProgramData;
	//Program data queue
	typedef std::shared_ptr<std::vector<std::queue<ProgramData>>> PDQueue;

	typedef std::shared_ptr<Address> AddressPtr;

	class Node
	{
	public:
		DLL_EXPORT Node(PDQueue& dataQueue, TQueue recieve, TQueue send, AddressPtr& addr);

		DLL_EXPORT void Update();

		DLL_EXPORT AckData GetLocalPackageData() const;
		DLL_EXPORT void ConnectRecieveQueue(TQueue& recieve);
		DLL_EXPORT void ConnectSendQueue(TQueue& send);
	private:

		enum Error
		{
			UnknownInvalid,
			KnownInvalid,
			SendingQueueNull,
			RecieveQueueNull
		};

		bool SendPacket();
		bool RecievePacket();

		//Look at ErrorType for values
		std::bitset<8> IsNodeValid();
		TQueue m_recievingQueue;
		TQueue m_sendingQueue;
		PDQueue m_dataQueue;
		AddressPtr m_connectionAddress;

		void IncrementLocal();
		void PollRecievedData(EPacket* sequence);

		//Ack = Acknowledged
		AckData m_LocalAckData;
		uint32_t m_RemoteAckData = 0;

		// CSP = Current sequence position
		uint32_t m_LocalCSP = 0;
		uint32_t m_RemoteCSP = 0;
	};


}