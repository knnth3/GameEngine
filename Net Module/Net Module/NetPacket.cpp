#include "NetPacket.h"
#include "NetModule.h"

namespace Net
{
	NetPacket::NetPacket()
	{
		m_sequence = 0;
		m_acknowledged = 0;
	}

	NetPacket::NetPacket(const Address& address, uint32_t sequence, uint32_t acknowledged)
	{
		m_address = address;
		if (!address.Empty())
		{
			m_sequence = sequence;
			m_acknowledged = acknowledged;
		}
	}

	bool NetPacket::AddData(const ByteBuffer& data)
	{
		m_buffer = data;
		return true;
	}

	void NetPacket::AddBitfield(uint32_t bitfield)
	{
	}

	const Address& NetPacket::GetAddress() const
	{
		return m_address;
	}

	uint32_t NetPacket::GetSequence()const
	{
		return m_sequence;
	}

	uint32_t NetPacket::GetAcknowledged()const
	{
		return m_acknowledged;
	}

	uint32_t NetPacket::GetBitfield()const
	{
		return m_bitfield;
	}

	const ByteBuffer & NetPacket::data() const
	{
		return m_buffer;
	}
}