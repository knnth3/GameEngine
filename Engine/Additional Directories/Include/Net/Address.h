#pragma once
#include <string>
#include <bitset>
#include "helper.h"

namespace Net
{
	class Address
	{
	public:

		DLL_EXPORT Address(unsigned int a,
			unsigned int b,
			unsigned int c,
			unsigned int d,
			unsigned short port, uint16_t id);

		DLL_EXPORT Address(unsigned int address, unsigned short port, uint16_t id);
		DLL_EXPORT Address(std::string address, unsigned short port, uint16_t id);

		DLL_EXPORT bool AssignName(std::string name);
		DLL_EXPORT bool AssignID(uint32_t id);
		DLL_EXPORT unsigned int GetAddress() const;
		DLL_EXPORT std::string GetAddressString() const;
		DLL_EXPORT unsigned short GetPort() const;
		DLL_EXPORT uint16_t GetID();
		DLL_EXPORT std::string GetName();

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
}

