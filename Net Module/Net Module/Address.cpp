#include "Address.h"
#include <vector>



Address::Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port, uint16_t id)
{
	unsigned int address = EncodeAddress(a, b, c, d);
	init(address, port, id);
}

Address::Address(unsigned int address, unsigned short port, uint16_t id)
{
	init(address, port, id);
}

Address::Address(std::string address, unsigned short port, uint16_t id)
{
	size_t pos = 0;
	std::string breakpoint = ".";
	//Loopback address by default fallback

	std::vector<unsigned int> newAddr;

	std::string section;
	while ((pos = address.find(breakpoint)) != std::string::npos)
	{
		section = address.substr(0, pos);
		unsigned int num = std::stoi(section);
		newAddr.push_back(num);
		address.erase(0, pos + breakpoint.length());
	}
	newAddr.push_back(std::stoi(address));

	if (newAddr.size() != 4)
	{
		unsigned int address = EncodeAddress(127, 0, 0, 1);
		init(address, port, id);
	}
	else
	{
		unsigned int address = EncodeAddress(newAddr[0], newAddr[1], newAddr[2], newAddr[3]);
		init(address, port, id);
	}
}

bool Address::AssignName(std::string name)
{
	if (m_name.size() == 0)
	{
		m_name = name;
		return true;
	}
	return false;
}

bool Address::AssignID(uint32_t id)
{
	if (m_ID == 0)
	{
		m_ID = id;
		return true;
	}
	return false;
}

unsigned int Address::GetAddress() const
{
	return m_address;
}

std::string Address::GetAddressString() const
{
	int a = m_address >> 24;
	int b = (m_address << 8) >> 24;
	int c = (m_address << 16) >> 24;
	int d = (m_address << 24) >> 24;
	std::string addr = std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
	return addr;
}

unsigned short Address::GetPort() const
{
	return m_port;
}

uint16_t Address::GetID()
{
	return m_ID;
}

std::string Address::GetName()
{
	return m_name;
}

unsigned int Address::EncodeAddress(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
	return (a << 24) | (b << 16) | (c << 8) | d;
}

void Address::init(unsigned int address, unsigned short port, uint16_t id)
{
	m_address = address;
	m_port = port;
	m_ID = id;
}
