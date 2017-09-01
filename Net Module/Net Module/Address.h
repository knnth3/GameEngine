#pragma once
#include <string>
#include <bitset>

class Address
{
public:

	Address(unsigned int a,
		unsigned int b,
		unsigned int c,
		unsigned int d,
		unsigned short port, uint16_t id);

	Address(unsigned int address, unsigned short port, uint16_t id);
	Address(std::string address, unsigned short port, uint16_t id);

	bool AssignName(std::string name);
	bool AssignID(uint32_t id);
	unsigned int GetAddress() const;
	std::string GetAddressString() const;
	unsigned short GetPort() const;
	uint16_t GetID();
	std::string GetName();

private:

	unsigned int EncodeAddress(unsigned int a,
		unsigned int b,
		unsigned int c,
		unsigned int d);
	void init(unsigned int address, unsigned short port, uint16_t id);
	std::string m_name = "";
	uint32_t m_ID = 0;
	unsigned int m_address = 0;
	unsigned short m_port = 0;
};

