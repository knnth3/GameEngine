#include "Server.h"


Lime::Server::Server(short Port)
{
	m_Server = std::make_unique<Net::Server>(Port);
}

void Lime::Server::Initialize()
{
	m_Server->Initialize();
}

void Lime::Server::Send(int ID, std::vector<byte>& data)
{
	ProgramData d = std::make_shared<edata>(data);
	m_Server->Send(ID, d);
}

bool Lime::Server::Recieve(int ID, std::vector<byte>& data)
{
	bool result = false;
	ProgramData d = std::make_shared<edata>();
	result = m_Server->Recieve(ID, d);
	if (result)
	{
		data = *d.get();
	}
	return result;
}

uint16_t Lime::Server::GetID(std::string username)
{
	return m_Server->GetAddress(username).GetID();
}

void Lime::Server::GetConnectedUsers(std::vector<std::string>& usernames)
{
	m_Server->GetConnectedUsers(usernames);
}

void Lime::Server::Close()
{
	m_Server->Close();
}

