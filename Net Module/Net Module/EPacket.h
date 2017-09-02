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
		DLL_EXPORT EPacket(std::shared_ptr<Address>& addr, uint32_t sequence, uint32_t acknowledged);
		DLL_EXPORT EPacket(edata rawData);

		DLL_EXPORT bool AddData(const void* data, int size);
		DLL_EXPORT void AddBitfield(uint32_t bitfield);

		DLL_EXPORT std::shared_ptr<Address> GetAddress()const;
		DLL_EXPORT ProgramData& GetBuffer();
		DLL_EXPORT uint32_t GetSequence();
		DLL_EXPORT uint32_t GetAcknowledged();
		DLL_EXPORT uint32_t GetBitfield();

		DLL_EXPORT ProgramData data();

	private:
		void AddProperty(ProgramData& packet, uint32_t value);
		void AddPacketData(ProgramData& packet, ProgramData& data);
		void GrabProperty(uint32_t& value, int& count, edata& data);
		void GrabData(ProgramData& data, edata& raw);

		std::shared_ptr<Address> m_addr;
		uint32_t m_sequence = 0;
		uint32_t m_acknowledged = 0;
		uint32_t m_bitfield = 0;
		ProgramData m_buffer;
	};

}
