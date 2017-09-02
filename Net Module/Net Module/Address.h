#pragma once
#include <string>
#include <bitset>
#include "NetModule.h"

namespace Net
{
	class Address
	{
	public:

		NET_API Address(unsigned int a,
				 	unsigned int b,
				 	unsigned int c,
				 	unsigned int d,
				 	unsigned short port, 
				 	Identification id);
				 
		NET_API Address(unsigned int address, unsigned short port, Identification id);
		NET_API Address(std::string address, unsigned short port, Identification id);

		NET_API bool AssignName(std::string name);
		NET_API bool AssignID(Identification id);
		NET_API unsigned int GetAddress() const;
		NET_API std::string GetAddressString() const;
		NET_API unsigned short GetPort() const;
		NET_API Identification GetID();
		NET_API std::string GetName();

	private:

		NET_API unsigned int EncodeAddress(unsigned int a,
			unsigned int b,
			unsigned int c,
			unsigned int d);
		NET_API void init(unsigned int address, unsigned short port, Identification id);
		std::string m_name = "";
		Identification m_ID = 0;
		unsigned int m_address = 0;
		unsigned short m_port = 0;
	};
}

