#include "Address.h"
#include <vector>

namespace Net
{
	Address::Address()
	{
		Initialize(0, 0);
	}

	Address::Address(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint16_t port)
	{
		unsigned int address = EncodeAddress(a, b, c, d);
		Initialize(address, port);
	}

	Address::Address(uint32_t address, uint16_t port)
	{
		Initialize(address, port);
	}

	Address::Address(const std::string& ipv4, uint16_t port)
	{
		size_t pos = 0;
		std::string breakpoint = ".";
		//Loopback address by default fallback

		std::vector<uint32_t> newAddr;

		std::string section;
		std::string address = ipv4;
		while ((pos = address.find(breakpoint)) != std::string::npos)
		{
			section = address.substr(0, pos);
			uint32_t num = std::stoi(section);
			newAddr.push_back(num);
			address.erase(0, pos + breakpoint.length());
		}
		newAddr.push_back(std::stoi(address));

		if (newAddr.size() != 4)
		{
			uint32_t address = EncodeAddress(127, 0, 0, 1);
			Initialize(address, port);
		}
		else
		{
			uint32_t address = EncodeAddress(newAddr[0], newAddr[1], newAddr[2], newAddr[3]);
			Initialize(address, port);
		}
	}

	uint32_t Address::GetPackedIPv4() const
	{
		return m_address;
	}

	std::string Address::GetStringIPv4() const
	{
		int a = m_address >> 24;
		int b = (m_address << 8) >> 24;
		int c = (m_address << 16) >> 24;
		int d = (m_address << 24) >> 24;
		std::string addr = std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
		return addr;
	}

	uint16_t Address::GetPort() const
	{
		return m_port;
	}

	bool Address::Empty() const
	{
		return (bool)m_address;
	}

	unsigned int Address::EncodeAddress(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
	{
		return (a << 24) | (b << 16) | (c << 8) | d;
	}

	void Address::Initialize(uint32_t address, uint16_t port)
	{
		m_address = address;
		m_port = port;
	}

}