#include "EPacket.h"
#include "Helper.h"

namespace Net
{


	EPacket::EPacket(std::shared_ptr<Address>& addr, uint32_t sequence, uint32_t acknowledged)
	{
		m_addr = addr;
		if (addr != nullptr)
		{
			m_buffer = std::make_shared<edata>();
			m_sequence = sequence;
			m_acknowledged = acknowledged;
		}
	}

	EPacket::EPacket(edata rawData)
	{
		int count = 0;
		GrabProperty(m_sequence, count, rawData);
		GrabProperty(m_acknowledged, count, rawData);
		GrabProperty(m_bitfield, count, rawData);
		GrabData(m_buffer, rawData);
	}

	bool EPacket::AddData(const void * data, int size)
	{
		if (m_addr != nullptr)
		{
			int csize = static_cast<int>(m_buffer->size());
			int totalSize = size + csize + PACKET_INFO_SIZE;
			if (totalSize > MAX_PACKET_SIZE)
				return false;

			m_buffer->resize(csize + size);
			memcpy(m_buffer->data() + csize, data, size);
			return true;
		}
		return false;
	}

	void EPacket::AddBitfield(uint32_t bitfield)
	{
		if (m_addr != nullptr)
		{
			m_bitfield = bitfield;
		}
	}

	std::shared_ptr<Address> EPacket::GetAddress() const
	{
		return m_addr;
	}

	ProgramData& EPacket::GetBuffer()
	{
		return m_buffer;
	}

	uint32_t EPacket::GetSequence()
	{
		return m_sequence;
	}

	uint32_t EPacket::GetAcknowledged()
	{
		return m_acknowledged;
	}

	uint32_t EPacket::GetBitfield()
	{
		return m_bitfield;
	}

	ProgramData EPacket::data()
	{
		ProgramData data = std::make_shared<edata>();
		AddProperty(data, m_sequence);
		AddProperty(data, m_acknowledged);
		AddProperty(data, m_bitfield);
		AddPacketData(data, m_buffer);
		return data;
	}

	void EPacket::AddProperty(ProgramData& packet, uint32_t value)
	{
		value = htonl(value);
		int currentSize = static_cast<int>(packet->size());
		packet->resize(currentSize + sizeof(uint32_t));
		for (size_t it = 0; it < sizeof(uint32_t); it++)
		{
			packet->at(currentSize + it) = value & 0xFF;
			value >>= 8;
		}
	}

	void EPacket::AddPacketData(ProgramData & packet, ProgramData & data)
	{
		int dataSize = static_cast<int>(data->size());
		int packetSize = static_cast<int>(packet->size());
		packet->resize(packetSize + dataSize);
		memcpy(packet->data() + packetSize, data->data(), dataSize);
	}

	void EPacket::GrabProperty(uint32_t & value, int& count, edata& data)
	{
		if (data.size() != 0)
		{
			int firstPiece = sizeof(uint32_t) - 1;
			for (int it = 0; it < firstPiece; it++)
			{
				int index = count + (firstPiece - it);
				value |= data[index];
				value <<= 8;
			}
			value |= data[count];
			value = ntohl(value);
			count += sizeof(uint32_t);
		}
	}

	void EPacket::GrabData(ProgramData& data, edata & raw)
	{
		data = std::make_shared<edata>();
		if (raw.size() <= MAX_PACKET_SIZE)
		{
			int actualSize = static_cast<int>(raw.size()) - PACKET_INFO_SIZE;
			data->resize(actualSize);
			memcpy(data->data(), raw.data() + PACKET_INFO_SIZE, actualSize);
		}
	}

}