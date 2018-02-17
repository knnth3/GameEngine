#pragma once
#include <string>
#include <bitset>
#include "NetModule.h"

namespace Net
{
	class Address
	{
	public:

		NET_API Address();
		NET_API Address(uint32_t a, uint32_t b, uint32_t c,
			uint32_t d, uint16_t port);
		NET_API Address(uint32_t ipv4, uint16_t port);
		NET_API Address(const std::string& address, uint16_t port);
		NET_API uint32_t GetPackedIPv4() const;
		NET_API std::string GetStringIPv4() const;
		NET_API uint16_t GetPort() const;
		NET_API bool Empty()const;

	private:

		unsigned int EncodeAddress(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
		void Initialize(uint32_t address, uint16_t port);

		uint32_t m_address = 0;
		uint16_t m_port = 0;
	};
}

