#pragma once
#include "Address.h"
#include <vector>
#include <memory>

#define MAX_PACKET_SIZE 1024
#define PACKET_INFO_SIZE 12

typedef unsigned char byte;
typedef std::vector<byte> edata;
typedef std::shared_ptr<edata> ProgramData;

struct EPacket
{
public:
	EPacket(std::shared_ptr<Address>& addr, uint32_t sequence, uint32_t acknowledged);
	EPacket(edata rawData);

	bool AddData(const void* data , int size);
	void AddBitfield(uint32_t bitfield);

	std::shared_ptr<Address> GetAddress()const;
	ProgramData& GetBuffer();
	uint32_t GetSequence();
	uint32_t GetAcknowledged();
	uint32_t GetBitfield();

	ProgramData data();

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

