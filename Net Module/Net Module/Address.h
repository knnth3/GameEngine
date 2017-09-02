#pragma once
#include <string>
#include <bitset>
#include "helper.h"

namespace Net
{
	class Address
	{
	public:

		Address(unsigned int a,
			unsigned int b,
			unsigned int c,
			unsigned int d,
			unsigned short port, 
			Identification id);

		Address(unsigned int address, unsigned short port, Identification id);
		Address(std::string address, unsigned short port, Identification id);

		bool AssignName(std::string name);
		bool AssignID(Identification id);
		unsigned int GetAddress() const;
		std::string GetAddressString() const;
		unsigned short GetPort() const;
		Identification GetID();
		std::string GetName();

	private:

		unsigned int EncodeAddress(unsigned int a,
			unsigned int b,
			unsigned int c,
			unsigned int d);
		void init(unsigned int address, unsigned short port, Identification id);
		std::string m_name = "";
		Identification m_ID = 0;
		unsigned int m_address = 0;
		unsigned short m_port = 0;
	};
}

