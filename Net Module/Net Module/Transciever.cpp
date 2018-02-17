#include "Transciever.h"
#include <locale>
#include <codecvt>

using namespace std::placeholders;
using namespace Net;

Transciever::Transciever()
{
	m_bInit = false;
	m_closeThread = false;
	m_socket = std::make_unique<UDPSocket>();
}

bool Transciever::Initialize(unsigned short port, bool isClient)
{
if (isClient)
port += 1;

m_bInit = m_socket->Open(port);
if (m_bInit)
{
	m_asyncThread = std::async(std::launch::async, &Transciever::Thread_Update_Func, this);
}
return m_bInit;
}

void Transciever::Send(const uint32_t & address, const ByteBuffer & buffer)
{
	std::lock_guard<std::mutex> lock(m_lock);
	auto found = m_activeNodes.find(address);
	if (found != m_activeNodes.end())
	{
		found->second->SetNextPacket(buffer);
	}
}

void Net::Transciever::SendToAll(const ByteBuffer & buffer)
{
	std::lock_guard<std::mutex> lock(m_lock);
	for (auto& n : m_activeNodes)
	{
		auto found = m_activeNodes.find(n.second->GetAddress().GetPackedIPv4());
		if (found != m_activeNodes.end())
		{
			found->second->SetNextPacket(buffer);
		}
	}
}

bool Transciever::Recieve(const uint32_t & address, ByteBuffer & buffer)
{
	std::lock_guard<std::mutex> lock(m_lock);
	auto found = m_activeNodes.find(address);
	if (found != m_activeNodes.end())
	{
		return found->second->GetNextPacket(buffer);
	}
	return false;
}

bool Net::Transciever::RecieveAll(std::vector<SimplePacket>& buffers)
{
	std::lock_guard<std::mutex> lock(m_lock);
	for (auto& user : m_activeNodes)
	{
		SimplePacket packet;
		packet.Address = user.first;
		user.second->GetNextPacket(packet.Buffer);

		if (!packet.Buffer.empty())
			buffers.push_back(packet);
	}

	return !buffers.empty();
}

void Net::Transciever::SendLoginRequest(const LoginPacket& packet)
{
	SimplePacket request;
	std::string data = "-LOGIN:" + packet.Username + " " + packet.Password + " " + packet.Answer;
	request.Address = packet.Address;
	request.Port = packet.Port;
	request.Buffer.insert(request.Buffer.begin(), data.begin(), data.end());
	m_sendLoginReqs.push(request);
}

bool Net::Transciever::GetLoginRequest(LoginPacket& packet)
{
	if (!m_recvdLoginReqs.empty())
	{
		auto request = m_recvdLoginReqs.front();
		m_recvdLoginReqs.pop();
		std::string command(request.Buffer.begin(), request.Buffer.end());
		if (!command.empty() && command.at(0) == '-')
		{
			auto end = command.find_first_of(':');
			if (end != std::string::npos)
			{
				std::string cmd = command.substr(1, end - 1);
				if (!cmd.compare("LOGIN"))
				{
					auto msgBreak = command.find_first_of(' ', end);
					auto msgBreak2 = command.find_first_of(' ', msgBreak + 1);
					if (msgBreak != std::string::npos && msgBreak2 != std::string::npos)
					{
						packet.Address = request.Address;
						packet.Port = request.Port;
						packet.Username = command.substr(end + 1, msgBreak - end - 1);
						packet.Password = command.substr(msgBreak + 1, msgBreak2 - msgBreak - 1);
						packet.Answer = command.substr(msgBreak2 + 1);
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Net::Transciever::SendLogoutRequest(const uint32_t & address)
{
	std::string data = "-LOGOUT:";
	ByteBuffer buffer(data.begin(), data.end());
	Send(address, buffer);
}

void Net::Transciever::SendLogoutRequestToAll()
{
	std::string data = "-LOGOUT:";
	ByteBuffer buffer(data.begin(), data.end());
	std::lock_guard<std::mutex> lock(m_lock);
	for (auto& n : m_activeNodes)
	{
		auto found = m_activeNodes.find(n.second->GetAddress().GetPackedIPv4());
		if (found != m_activeNodes.end())
		{
			found->second->SetNextPacket(buffer);
		}
	}
}

bool Net::Transciever::GetLogoutRequest(uint32_t & address)
{
	if(!m_logoutRequests.empty())
	{
		address = m_logoutRequests.front();
		m_logoutRequests.pop();
		return true;
	}
	return false;
}

bool Net::Transciever::CreateNode(const uint32_t& address, const uint16_t port, const std::string& name)
{
	std::lock_guard<std::mutex> lock(m_lock);
	if (m_activeNodes.find(address) == m_activeNodes.end())
	{
		Address ipv4(address, port);
		m_activeNodes.emplace(std::pair<uint32_t, std::unique_ptr<Node>>(address, std::make_unique<Node>(ipv4, name)));
		return true;
	}

	return false;
}

void Transciever::Close()
{
	if (m_bInit)
	{
		using namespace std::chrono_literals;
		m_closeThread = true;
		std::future_status status;
		do
		{
			status = m_asyncThread.wait_for(0ms);
		} while (status != std::future_status::ready);
		m_socket->Close();
	}
}

void Transciever::Thread_Update_Func()
{
	while (!m_closeThread)
	{
		RecievePacket();
		SendPackets();
	}
}

void Transciever::RecievePacket()
{
	if (m_bInit)
	{
		//Create container big enough to hold incoming data
		ByteBuffer recvd;
		recvd.resize(MAX_PACKET_SIZE);
		Address peer;

		//Recieve incoming data (if any)
		int bytes_recieved = m_socket->Receive(peer, recvd.data(), MAX_PACKET_SIZE);

		if (bytes_recieved > 0)
		{
			auto packet = std::make_shared<NetPacket>(peer, 0 ,0);
			recvd.resize(bytes_recieved);
			packet->AddData(recvd);
			//Put data in designated queueue
			PlacePacket(packet);
		}
	}
}

void Transciever::SendPackets()
{
	if (m_bInit)
	{

		for (int index = 0; index < m_sendLoginReqs.size(); index++)
		{
			auto packet = m_sendLoginReqs.front();
			m_sendLoginReqs.pop();

			Address address(packet.Address, packet.Port);
			int bytes_sent = m_socket->Send(address, packet.Buffer.data(), (int)(packet.Buffer.size()));
		}

		std::lock_guard<std::mutex> lock(m_lock);
		for (const auto& node : m_activeNodes)
		{
			std::shared_ptr<NetPacket> packet;
			if (node.second->GetNextSendingPacket(packet))
			{
				auto data = packet->data();
				auto address = packet->GetAddress();
				int bytes_sent = m_socket->Send(address, data.data(), (int)(data.size()));
			}
		}
	}
}

void Transciever::PlacePacket(std::shared_ptr<NetPacket>& packet)
{
	std::string command;
	uint32_t ID = packet->GetAddress().GetPackedIPv4();
	std::lock_guard<std::mutex> lock(m_lock);
	if (packet->data().size() >= 8)
	{
		command.insert(command.begin(), packet->data().begin(), packet->data().begin() + 8);
	}
	if (!command.compare("-LOGOUT:"))
	{
		m_activeNodes.erase(ID);
		m_logoutRequests.push(ID);
	}
	else if (m_activeNodes.find(ID) != m_activeNodes.end())
	{
		m_activeNodes.at(ID)->AddRecievedPacket(packet);
	}
	else
	{
		SimplePacket request;
		request.Address = packet->GetAddress().GetPackedIPv4();
		request.Port = packet->GetAddress().GetPort();
		request.Buffer = packet->data();
		m_recvdLoginReqs.push(request);
	}
}
