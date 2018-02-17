#include "Server.h"
#include "Address.h"

namespace Net
{

	using namespace std::placeholders;

	Server::Server(const std::string& password)
	{
		InitializeSockets();
		m_bIsInit = false;
		m_transciever = std::make_unique<Transciever>();
		m_password = password;
	}

	Server::~Server()
	{
		if (m_bIsInit)
		{
			m_transciever->Close();
			ShutdownSockets();
		}
	}

	bool Server::Open(uint16_t port)
	{
		m_bIsInit = m_transciever->Initialize(port);
		return m_bIsInit;
	}

	void Server::SetPassword(const std::string & password)
	{
		m_password = password;
	}

	bool Server::Send(const uint32_t& address, const std::string & data)
	{
		if (m_bIsInit)
		{
			ByteBuffer buffer(data.begin(), data.end());
			m_transciever->Send(address, buffer);
			return true;
		}
		return false;
	}

	bool Server::Send(const uint32_t& address, const ByteBuffer & data)
	{
		if (m_bIsInit)
		{
			m_transciever->Send(address, data);
			return true;
		}
		return false;
	}

	bool Server::SendToAll(const ByteBuffer & data)
	{
		if (m_bIsInit)
		{
			m_transciever->SendToAll(data);
			return true;
		}
		return false;
	}

	bool Server::Recieve(const uint32_t& address, ByteBuffer& packet)
	{
		if (m_bIsInit)
		{
			return m_transciever->Recieve(address, packet);
		}
		return false;
	}

	bool Server::RecieveAll(std::vector<SimplePacket>& recieved)
	{
		Address address;
		ByteBuffer buffer;
		m_transciever->RecieveAll(recieved);
		return !recieved.empty();
	}

	bool Server::GetNewLoginRequest(uint32_t& ID, std::string & username)
	{
		LoginPacket packet;
		if (m_transciever->GetLoginRequest(packet))
		{
			if (m_password.empty())
			{
				ID = packet.Address;
				username = packet.Username;
				if (m_transciever->CreateNode(packet.Address, packet.Port, packet.Username))
				{
					packet.Answer = "TRUE";
					m_transciever->SendLoginRequest(packet);
					return true;
				}
				else
				{
					packet.Answer = "User_already_exists";
					m_transciever->SendLoginRequest(packet);
				}
			}
			else if (!packet.Password.compare(m_password))
			{
				//Password correct
				ID = packet.Address;
				username = packet.Username;
				if (m_transciever->CreateNode(packet.Address, packet.Port, packet.Username))
				{
					packet.Answer = "TRUE";
					m_transciever->SendLoginRequest(packet);
					return true;
				}
				else
				{
					packet.Answer = "User_already_exists";
					m_transciever->SendLoginRequest(packet);
				}
			}
		}

		return false;
	}

	bool Server::GetLogoutRequest(uint32_t & address)
	{
		if (m_bIsInit)
		{
			return m_transciever->GetLogoutRequest(address);
		}
		return false;
	}

	void Server::CloseConnection(uint32_t & address)
	{
		m_transciever->SendLogoutRequest(address);
	}

	void Server::CloseAllConnections()
	{
		m_transciever->SendLogoutRequestToAll();
	}
}
