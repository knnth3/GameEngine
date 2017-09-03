#include "Transciever.h"
#include <chrono>


namespace Net
{

	Transciever::Transciever(unsigned short port)
	{
		m_port = port;
		m_socket = std::make_unique<UDPSocket>();
	}

	bool Transciever::Init(std::shared_ptr<TDataBase>& recievedDB, TQueue sending)
	{
		bool isSuccess = m_socket->Open(m_port);
		if (isSuccess)
		{
			m_recievedDB = recievedDB;
			if (m_recievedDB == nullptr)
			{
				isSuccess = false;
				printf("Could not connect to Nodes Database!\n");
			}
			else
			{
				m_sendingDB = sending;
				if (m_sendingDB == nullptr)
				{
					isSuccess = false;
					printf("Could not connect to Nodes sending queue!\n");
				}
			}
		}
		return isSuccess;
	}

	void Transciever::SetConnectionFunc(AddFunc connectionFunc)
	{
		m_ConnectionFunc = connectionFunc;
	}

	void Transciever::Update()
	{
		RecievePacket();
		SendPacket();
	}

	void Transciever::Close()
	{
		m_socket->Close();
	}

	void Transciever::RecievePacket()
	{
		if (isValid())
		{
			//Create container big enough to hold incoming data
			edata raw;
			raw.resize(MAX_PACKET_SIZE);
			std::shared_ptr<Address> peer;

			//Recieve incoming data (if any)
			int bytes_recieved = m_socket->Receive(peer, raw.data(), MAX_PACKET_SIZE);

			if (bytes_recieved > 0)
			{
				raw.resize(bytes_recieved);
				EPacket packet(raw);
				//Put data in designated queueue
				PlacePacket(peer, packet);
			}
		}
	}

	void Transciever::SendPacket()
	{
		auto packet = GetPacket();
		if (packet)
		{
			auto data = packet->data();
			auto address = packet->GetAddress();
			int bytes_sent = m_socket->Send(address, data->data(), static_cast<int>(data->size()));
		}
	}

	bool Transciever::isValid()
	{
		if (m_sendingDB == nullptr || m_recievedDB == nullptr)
		{
			return false;
		}
		return true;
	}

	std::shared_ptr<EPacket> Transciever::GetPacket()
	{
		if (isValid())
		{
			if (!m_sendingDB->empty())
			{
				//Grab next packet
				EPacket packet = m_sendingDB->front();
				m_sendingDB->pop();

				return std::make_shared<EPacket>(packet);
			}
		}
		return nullptr;
	}

	void Transciever::PlacePacket(std::shared_ptr<Address>& addr, EPacket& packet)
	{
		//Check to see if the packet recieved is to initiate connection.
		ConnectionType result = ConnectPeer(addr, packet.GetBuffer());
		Identification ID = addr->GetID();
		switch (result)
		{
		case ConnectionType::NewConnect:
			break;

		case ConnectionType::Success:
			if (m_recievedDB->find(ID) != m_recievedDB->end())
			{
				m_recievedDB->at(ID)->push(packet);
			}
			break;
		case ConnectionType::Failed:
			printf("Authentication failed for user IP:%s \n", addr->GetAddressString().c_str());
			break;

		default:
			printf("ID# %i sent an invalid packet! Packet will be discarded.\n", ID);
			break;
		}
	}

	ConnectionType Transciever::ConnectPeer(std::shared_ptr<Address>& addr, ProgramData& packet)
	{

		if (m_ConnectionFunc != nullptr)
		{
			return m_ConnectionFunc(addr, packet);
		}
		else
			return ConnectionType::Failed;
	}
}
