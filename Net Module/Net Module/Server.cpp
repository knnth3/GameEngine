#include "Server.h"
#include "Address.h"
#include "EPacket.h"

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

Address Server::GetAddress(std::string username)
{
	//For user purpose
	return *m_connections[username];
}

void Server::Send(int ID, ProgramData & info)
{
	if (IsValid(ID))
	{
		m_database->at(ID)->at(outqueue).push(info);
	}
}

bool Server::Recieve(int ID, ProgramData & info)
{
	if (IsValid(ID))
	{
		if (!m_database->at(ID)->at(inqueue).empty())
		{
			info = m_database->at(ID)->at(inqueue).front();
			m_database->at(ID)->at(inqueue).pop();
			return true;
		}
	}
	return false;
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
	uint32_t id = address->GetID();
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
		uint32_t key = 0;
		uint32_t seed = 0;
		memcpy(&key, data->data(), 4);
		for (size_t x = 4; x < data->size(); x++)
		{
			seed <<= 8;
			seed |= data->at(x);
		}

		uint32_t value = htonl(GenerateKey(seed));
		if (value == key)
		{
			address->AssignID(ntohl(key));
			std::string username = std::string(data->begin() + 4, data->end());
			address->AssignName(username);
			m_connections[username] = address;
			CreateNode(address);

			edata data = { 'H','e','y','!',' ','Y','o','u',' ','L','o','g','g','e','d',' ','i','n','!','\n' };
			ProgramData loginData = std::make_shared<edata>(data);
			Send(m_connections[username]->GetID(), loginData);
		}
		else
		{
			printf("Login failed, Invalid Key!\n");
			return ConnectionType::Failed;
		}
	}
	return ConnectionType::NewConnect;
}

uint32_t Server::GenerateKey(unsigned int seed)
{
	return ((((seed/2) + 5724) % 100001) >> 8);
}

bool Server::IsValid(uint16_t ID)
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

