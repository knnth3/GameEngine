#include "Node.h"

namespace Net
{

	Node::Node(PDQueue& dataQueue, TQueue recieve, TQueue send, AddressPtr& addr)
	{
		m_connectionAddress = addr;
		if (m_connectionAddress == nullptr)
			printf("Node created for Connection -nullptr-\n");
		else
			printf("Node created successfully!\n");

		m_dataQueue = dataQueue;
		if (m_dataQueue == nullptr)
			printf("Node did not attach to server data queue!\n");
		else
		{
			m_dataQueue->resize(2);
		}
		m_sendingQueue = send;
		m_recievingQueue = recieve;
	}

	void Node::Update()
	{
		RecievePacket();
		SendPacket();
	}

	bool Node::SendPacket()
	{
		std::bitset<8> result = IsNodeValid();
		bool statement1 = result.count() <= 1 && result[Error::RecieveQueueNull] == true;
		bool statement2 = result.none();
		if (statement1 || statement2)
		{
			//Grab the next packet (if any) and pops the result
			bool empty = m_dataQueue->at(outqueue).empty();
			if (!empty)
			{
				ProgramData data = m_dataQueue->at(outqueue).front();
				m_dataQueue->at(outqueue).pop();

				//Increments a counter to represent is most up-to-date packet
				IncrementLocal();

				//Pack data to be sent
				EPacket packet(m_connectionAddress, m_LocalCSP, m_RemoteCSP);
				packet.AddData(data->data(), static_cast<int>(data->size()));
				packet.AddBitfield(m_RemoteAckData);

				//Send Packet
				m_sendingQueue->push(packet);

				return true;
			}

		}
		return false;
	}

	bool Node::RecievePacket()
	{
		std::bitset<8> result = IsNodeValid();
		bool statement1 = result.count() <= 1 && result[Error::SendingQueueNull] == true;
		bool statement2 = result.none();
		if (statement1 || statement2)
		{
			if (!m_recievingQueue->empty())
			{
				//Grab next packet
				EPacket packet = m_recievingQueue->front();
				m_recievingQueue->pop();


				//Log arrival
				PollRecievedData(&packet);

				//Extract data to and send to server
				ProgramData data = packet.GetBuffer();
				m_dataQueue->at(inqueue).push(data);
				std::string datatest(data->begin(), data->end());
				printf("%s: %s\n", m_connectionAddress->GetName().c_str(), datatest.c_str());
				return true;
			}
		}
		return false;
	}

	AckData Node::GetLocalPackageData() const
	{
		return m_LocalAckData;
	}

	void Node::ConnectRecieveQueue(TQueue & recieve)
	{
		m_recievingQueue = recieve;
	}

	void Node::ConnectSendQueue(TQueue & send)
	{
		m_sendingQueue = send;
	}

	std::bitset<8> Node::IsNodeValid()
	{
		std::bitset<8> result = 0;
		if (m_dataQueue == nullptr)
		{
			if (m_connectionAddress == nullptr)
			{
				printf("A node is not valid! Ignoring its send/recieve request.\n");
				result.set(UnknownInvalid);
			}
			printf("Node for ID#%i is not valid! Ignoring send/recieve request.\n", m_connectionAddress->GetID());
			result.set(KnownInvalid);
		}
		if (m_sendingQueue == nullptr)
		{
			result.set(SendingQueueNull);
		}
		if (m_recievingQueue == nullptr)
		{
			result.set(RecieveQueueNull);
		}

		return result;
	}

	void Node::IncrementLocal()
	{
		m_LocalCSP++;
	}

	void Node::PollRecievedData(EPacket* packet)
	{
		//Saves all the data recieved.
		m_LocalAckData =
		{
			packet->GetAcknowledged(),
			packet->GetBitfield(),
		};

		uint32_t sequence = packet->GetSequence();

		//Updates the Remote CSP and the bitfield along with.
		if (m_RemoteCSP < sequence)
		{

			if (m_RemoteCSP != 0)
			{
				int pos = (m_RemoteCSP - 1) % 32;
				uint32_t bit = (uint32_t)pow(2, pos);
				m_RemoteAckData <<= 1;
				m_RemoteAckData |= bit;
			}

			m_RemoteCSP = sequence;
		}
		else
		{
			if (m_RemoteCSP != 0)
			{
				int pos = (sequence - 1) % 32;
				uint32_t bit = (uint32_t)pow(2, pos);
				m_RemoteAckData |= bit;
			}
		}
	}

}