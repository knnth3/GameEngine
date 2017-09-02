#include "Server.h"
#include "Address.h"
#include "EPacket.h"

namespace Net
{

	using namespace std::placeholders;

	Server::Server(int port)
	{
		InitializeSockets();
		//Init all data storage and sectors
		m_database = std::make_shared<DataBase>();
		m_recievedDB = std::make_shared<TDataBase>();
		m_sendingQueue = std::make_shared<std::queue<EPacket>>();
		m_ConnectionTimers = std::make_unique<IPAuthenticator>();
		m_transciever = std::make_unique<Transciever>(port);

		m_isInit = false;
	}

	bool Server::Initialize()
	{
		bool result = m_transciever->Init(m_recievedDB, m_sendingQueue);
		if (result)
		{
			m_transciever->SetConnectionFunc(std::bind(&Server::ConnectPeer, this, _1, _2));
			m_closeThread = false;
			m_asyncThread = std::async(std::launch::async, &Server::Update, this);
			m_isInit = true;
		}
		return result;
	}

	void Server::Update()
	{
		while (!m_closeThread)
		{
			m_transciever->Update();
			//Remove this when nodes are threaded!!!
			for (size_t index = 0; index < m_ActiveNodes.size(); index++)
			{
				m_ActiveNodes.at(index)->Update();
			}
		}
	}

	Identification Server::GetID(std::string username)
	{
		return m_connections[username]->GetID();
	}

	void Server::Send(Identification ID, std::vector<byte> data)
	{
		if (IsValid(ID))
		{
			ProgramData d = std::make_shared<edata>(data);
			m_database->at(ID)->at(outqueue).push(d);
		}
	}

	std::vector<byte> Server::Recieve(Identification ID)
	{
		edata data;
		if (IsValid(ID))
		{
			if (!m_database->at(ID)->at(inqueue).empty())
			{
				data = *(m_database->at(ID)->at(inqueue).front()).get();
				m_database->at(ID)->at(inqueue).pop();
			}
		}
		return data;
	}

	void Server::GetConnectedUsers(std::vector<std::string>& usernames)
	{
		for each(auto user in m_connections)
		{
			usernames.push_back(user.first);
		}
	}

	void Server::Close()
	{
		if (m_isInit)
		{
			using namespace std::chrono_literals;
			m_closeThread = true;
			std::future_status status;
			do
			{
				status = m_asyncThread.wait_for(0ms);
			} while (status != std::future_status::ready);

			m_transciever->Close();
			ShutdownSockets();
		}
	}

	void Server::CreateNode(std::shared_ptr<Address>& address)
	{
		//Create threads here for optimal work
		Identification id = address->GetID();
		m_database->emplace(id, std::make_shared<std::vector<std::queue<ProgramData>>>());
		m_recievedDB->emplace(id, std::make_shared<std::queue<EPacket>>());
		m_ActiveNodes.emplace_back
		(
			std::make_unique<Node>
			(
				m_database->at(id),
				m_recievedDB->at(id),
				m_sendingQueue,
				address
				)
		);
	}

	ConnectionType Server::ConnectPeer(std::shared_ptr<Address>& address, ProgramData& data)
	{
		if (IsAddressQueried(address))
		{
			return ConnectionType::Success;
		}
		if (!m_ConnectionTimers->IsTimedOut(address->GetAddress()))
		{
			if (data->size() <= 6)
			{
				printf("Invalid packet recieved.\n");
				return ConnectionType::Failed;
			}
			Identification key = 0;
			Identification seed = 0;
			memcpy(&key, data->data(), 4);
			for (size_t x = 4; x < data->size(); x++)
			{
				seed <<= 8;
				seed |= data->at(x);
			}

			Identification value = htons(GenerateKey(seed));
			if (value == key)
			{
				address->AssignID(ntohs(key));
				std::string username = std::string(data->begin() + 4, data->end());
				address->AssignName(username);
				m_connections[username] = address;
				CreateNode(address);

				edata data = { 'H','e','y','!',' ','Y','o','u',' ','L','o','g','g','e','d',' ','i','n','!','\n' };
				Send(m_connections[username]->GetID(), data);
			}
			else
			{
				printf("Login failed, Invalid Key!\n");
				return ConnectionType::Failed;
			}
		}
		return ConnectionType::NewConnect;
	}

	Identification Server::GenerateKey(Identification seed)
	{
		return ((((seed / 2) + 5724) % 100001) >> 8);
	}

	bool Server::IsValid(Identification ID)
	{
		if (m_database->find(ID) == m_database->end())
		{
			return false;
		}
		return true;
	}

	bool Server::IsAddressQueried(std::shared_ptr<Address>& address)
	{
		for each(auto x in m_connections)
		{
			if (x.second->GetAddress() == address->GetAddress())
			{
				address->AssignID(x.second->GetID());
				return true;
			}
		}
		return false;
	}

}
