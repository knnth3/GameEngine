#include "Node.h"

namespace Net
{
	Node::Node(const Address& address, const std::string& name)
	{
		m_address = address;
		m_name = name;
	}

	void Node::AddSendPacket(const std::shared_ptr<NetPacket> & data)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_sending.push(data);
	}

	void Node::AddRecievedPacket(const std::shared_ptr<NetPacket> & data)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_recieved.push(data);
	}

	bool Node::GetNextSendingPacket(std::shared_ptr<NetPacket>& data)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		if (!m_sending.empty())
		{
			data = m_sending.front();
			m_sending.pop();
			return true;
		}

		return false;
	}

	const std::string & Node::GetName() const
	{
		return m_name;
	}

	const Address & Node::GetAddress() const
	{
		return m_address;
	}

	bool Node::GetNextPacket(ByteBuffer& data)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		if (!m_recieved.empty())
		{
			auto packet = m_recieved.front();
			m_recieved.pop();
			if(packet)
			{
				data = packet->data();
			}
			return !data.empty();
		}

		return false;
	}

	bool Node::SetNextPacket(const ByteBuffer & data)
	{
		if (!data.empty())
		{
			auto packet = std::make_shared<NetPacket>(m_address, m_LocalCSP, m_RemoteCSP);
			packet->AddData(data);
			std::lock_guard<std::mutex> lock(m_lock);
			m_sending.push(packet);
			return true;
		}

		return false;
	}
}