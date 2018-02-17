#include "Client.h"
#include <locale>
#include <codecvt>

namespace Net
{

	using namespace std::placeholders;
	using namespace std::chrono;
	using clock = std::chrono::high_resolution_clock;

	Client::Client()
	{
		InitializeSockets();
		m_bIsInit = false;
		m_transciever = std::make_unique<Transciever>();
	}

	Client::~Client()
	{
		if (m_bIsInit)
		{
			m_transciever->Close();
			ShutdownSockets();
		}
	}

	bool Client::Initialize(const std::string & address, uint16_t port)
	{
		m_address = Address(address, port);
		if (m_transciever->Initialize(port, true))
		{
			m_bIsInit = true;
			return true;
		}
		return false;
	}

	bool Client::Login(const std::string& username, const std::string& password, double timeout)
	{
		if (m_bIsInit)
		{
			if (IsStringValid(username) && IsStringValid(password))
			{
				clock::time_point start;

				LoginPacket request;
				request.Address = m_address.GetPackedIPv4();
				request.Port = m_address.GetPort();
				request.Username = username;
				request.Password = password;
				request.Answer = "";
				m_transciever->SendLoginRequest(request);

				start = clock::now();
				while ((duration_cast<milliseconds>(clock::now() - start).count() <= timeout))
				{
					if (m_transciever->GetLoginRequest(request))
					{
						if (!request.Answer.compare("TRUE"))
						{
							return m_transciever->CreateNode(m_address.GetPackedIPv4(), m_address.GetPort(), "SERVER");
						}
					}
				}
			}
		}
		return false;
	}

	void Client::Logout()
	{
		m_transciever->SendLogoutRequest(m_address.GetPackedIPv4());
	}

	bool Client::Send(const std::string & data)
	{
		if (m_bIsInit)
		{
			ByteBuffer buffer(data.begin(), data.end());
			m_transciever->Send(m_address.GetPackedIPv4(), buffer);
			return true;
		}
		return false;
	}

	bool Client::Send(const ByteBuffer& data)
	{
		if (m_bIsInit)
		{
			m_transciever->Send(m_address.GetPackedIPv4(), data);
			return true;
		}
		return false;
	}

	bool Client::Recieve(ByteBuffer& data)
	{
		if (m_bIsInit)
		{
			return m_transciever->Recieve(m_address.GetPackedIPv4(), data);
		}
		return false;
	}

	bool Client::GetLogoutRequest(uint32_t & address)
	{
		if (m_bIsInit)
		{
			return m_transciever->GetLogoutRequest(address);
		}
		return false;
	}

	bool Client::IsStringValid(const std::string & value)
	{
		bool bHasColon = value.find(':') == std::string::npos;
		bool bHasSpace = value.find(' ') == std::string::npos;
		bool bHasDash = value.find('-') == std::string::npos;

		if (bHasColon && bHasSpace && bHasDash)
			return true;

		return false;
	}

}