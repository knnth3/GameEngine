#pragma once
#include "Address.h"
#include <vector>
#include <memory>

#define MAX_PACKET_SIZE 1024
#define PACKET_INFO_SIZE 12

namespace Net
{
	typedef unsigned char byte;
	typedef std::vector<byte> ByteBuffer;

	struct NetPacket
	{
	public:
		NET_API NetPacket();
		NET_API NetPacket(const Address& address, uint32_t sequence, uint32_t acknowledged);
		NET_API bool AddData(const ByteBuffer& data);
		NET_API void AddBitfield(uint32_t bitfield);
		NET_API const Address& GetAddress()const;
		NET_API uint32_t GetSequence()const;
		NET_API uint32_t GetAcknowledged()const;
		NET_API uint32_t GetBitfield()const;
		NET_API const ByteBuffer& data()const;

	private:

		Address m_address;
		uint32_t m_sequence = 0;
		uint32_t m_acknowledged = 0;
		uint32_t m_bitfield = 0;
		ByteBuffer m_buffer;
	};

}
