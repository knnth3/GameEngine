#include "PacketManager.h"


namespace Lime
{

    void PacketManager::AddIncomingPacket(std::shared_ptr<Packet> packet)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
		if (packet != nullptr)
		{
			m_RecievedPackages.push(packet);
		}
    }

    std::shared_ptr<Packet> PacketManager::RetrieveIncomingPacket()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (m_RecievedPackages.size() != 0)
        {
            std::shared_ptr<Packet> packet = m_RecievedPackages.front();
			m_RecievedPackages.pop();
            return packet;
        }
        return nullptr;
    }

    void PacketManager::AddSendPacket(UINT ID, std::shared_ptr<Packet> packet)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_SendPackages[ID] = packet;
    }

    std::shared_ptr<Packet> PacketManager::RetireveSendPacket(UINT ID)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (m_SendPackages.find(ID) != m_SendPackages.end())
        {
            std::shared_ptr<Packet> packet = m_SendPackages[ID];
            m_SendPackages[ID] = nullptr;
            return packet;
        }
        return nullptr;
    }

}