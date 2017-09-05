#include "Client.h"
#include <locale>
#include <codecvt>

namespace Net
{

	using namespace std::placeholders;

	Client::Client(const char* address, unsigned short port)
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
		if (!m_loggedIn)
		{
			if (data->at(0) == '!')
			{
				std::string recieved(data->begin() + 1, data->end());
				printf("Client: %s\n", recieved.c_str());
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
		Identification key = htons(GenerateKey(CONNECTION_KEY));
		std::string cmd = '!' + std::to_string(key) + "|" + username;
		Send(cmd.c_str());
	}

	void Client::Send(const char* data)
	{
		std::string strdata(data);
		edata info(strdata.begin(), strdata.end());
		ProgramData packet = std::make_shared<edata>(info);
		m_Queue->at(outqueue).push(packet);
	}

	void Client::Recive(char* data, uint32_t maxSize)
	{
		edata info;
		if (!m_Queue->at(inqueue).empty())
		{
			ProgramData data = m_Queue->at(inqueue).front();
			m_Queue->at(inqueue).pop();
			info = *data.get();
		}

		std::string cmd(info.begin(), info.end());
		if (maxSize >= cmd.length())
		{
			memcpy(data, cmd.c_str(), maxSize);
		}
	}

	Identification Client::GenerateKey(Identification seed)
	{
		return ((((seed / 2) + 5724) % 100001) >> 8);
	}
}