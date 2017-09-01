#include "Client.h"
#include <locale>
#include <codecvt>

using namespace std::placeholders;

Client::Client(std::string address, unsigned short port)
{
	InitializeSockets();
	m_ServerAddress = std::make_shared<Address>(address, port, 0);
	m_Queue = std::make_shared<std::vector<std::queue<ProgramData>>>();
	m_recievedDB = std::make_shared<TDataBase>();
	m_sendingDB = std::make_shared<std::queue<EPacket>>();
	m_Transciever = std::make_unique<Transciever>(CLIENT_PORT);
	m_ClientNode = std::make_unique<Node>(m_Queue, nullptr, m_sendingDB, m_ServerAddress);

#if PLATFORM == PLATFORM_WINDOWS

	WCHAR buff[50];
	DWORD size = 50;
	GetComputerName(buff, &size);

	std::wstring name = std::wstring(buff);
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string computer_name = converter.to_bytes(name);
	Login(computer_name);

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	std::string computer_name = "Place_Holder";
	Login(computer_name);

#endif

	m_isInit = false;
	m_loggedIn = false;
}


void Client::Close()
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

		m_Transciever->Close();
		ShutdownSockets();
	}
}

bool Client::Initialize()
{
	bool result = m_Transciever->Init(m_recievedDB, m_sendingDB);
	if (result)
	{
		m_Transciever->SetConnectionFunc(std::bind(&Client::ConnectClient, this, _1, _2));
		m_closeThread = false;
		m_asyncThread = std::async(std::launch::async, &Client::Update, this);
		m_isInit = true;
	}
	return result;
}

void Client::Update()
{
	while (!m_closeThread)
	{
		m_Transciever->Update();
		m_ClientNode->Update();
	}
}

ConnectionType Client::ConnectClient(std::shared_ptr<Address>& address, ProgramData & data)
{
	int result = 0;
	if(!m_loggedIn)
	{
		edata ldata = { 'H','e','y','!',' ','Y','o','u',' ','L','o','g','g','e','d',' ','i','n','!','\n' };
		std::string recieved(data->begin(), data->end());
		std::string login(ldata.begin(), ldata.end());
		if (login.compare(recieved) == 0)
		{
			printf("Client: %s", recieved.c_str());
			m_loggedIn = true;
			m_recievedDB->emplace(0, std::make_shared<std::queue<EPacket>>());
			m_ClientNode->ConnectRecieveQueue(m_recievedDB->at(0));
			return ConnectionType::NewConnect;
		}
		else
		{
			printf("Client: Unable to Connect!");
			return ConnectionType::Failed;
		}
	}
	return ConnectionType::Success;
}

void Client::Login(std::string username)
{
	ProgramData data = std::make_shared<edata>();
	data->resize(sizeof(uint32_t) + username.size());
	uint32_t seed = 0;
	for (size_t x = 0; x < username.size(); x++)
	{
		seed <<= 8;
		seed |= username.at(x);
	}
	uint32_t key = htonl(GenerateKey(seed));
	std::memcpy(data->data(), &key, sizeof(uint32_t));
	std::memcpy(data->data() + 4, username.c_str(), username.size());
	m_Queue->at(outqueue).push(data);
}

void Client::Send(std::string str)
{
	if (m_loggedIn)
	{
		ProgramData data = std::make_shared<edata>();
		data->resize(str.size());
		std::memcpy(data->data(), str.c_str(), str.size());
		m_Queue->at(outqueue).push(data);
	}
	else
	{
		printf("Client: Not Connected! Packet not sent.\n");
	}
}

uint32_t Client::GenerateKey(uint32_t seed)
{
	return ((((seed / 2) + 5724) % 100001) >> 8);
}
