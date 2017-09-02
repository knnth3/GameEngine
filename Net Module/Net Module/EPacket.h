#pragma once
#include "Address.h"
#include <vector>
#include <memory>

#define MAX_PACKET_SIZE 1024
#define PACKET_INFO_SIZE 12

typedef unsigned char byte;
typedef std::vector<byte> edata;
typedef std::shared_ptr<edata> ProgramData;

namespace Net
{
	struct EPacket
	{
	public:
		NET_API EPacket(std::shared_ptr<Address>& addr, uint32_t sequence, uint32_t acknowledged);
		NET_API EPacket(edata rawData);

		NET_API bool AddData(const void* data, int size);
		NET_API void AddBitfield(uint32_t bitfield);

		NET_API std::shared_ptr<Address> GetAddress()const;
		NET_API ProgramData& GetBuffer();
		NET_API uint32_t GetSequence();
		NET_API uint32_t GetAcknowledged();
		NET_API uint32_t GetBitfield();

		NET_API ProgramData data();

	private:
		NET_API void AddProperty(ProgramData& packet, uint32_t value);
		NET_API void AddPacketData(ProgramData& packet, ProgramData& data);
		NET_API void GrabProperty(uint32_t& value, int& count, edata& data);
		NET_API void GrabData(ProgramData& data, edata& raw);

		std::shared_ptr<Address> m_addr;
		uint32_t m_sequence = 0;
		uint32_t m_acknowledged = 0;
		uint32_t m_bitfield = 0;
		ProgramData m_buffer;
	};

}
