#pragma once
#include <mutex>
#include <queue>
#include <map>
#include "Primitives.h"


namespace Lime
{
    class PacketManager
    {
    public:
        void PacketManager::AddIncomingPacket(std::shared_ptr<Packet> packet);
        std::shared_ptr<Packet> RetrieveIncomingPacket();

        void PacketManager::AddSendPacket(UINT ID, std::shared_ptr<Packet> packet);
        std::shared_ptr<Packet> RetireveSendPacket(UINT ID);
    private:

        std::mutex m_Mutex;
		std::queue<std::shared_ptr<Packet>> m_RecievedPackages;
        std::map<UINT, std::shared_ptr<Packet>> m_SendPackages;
    };


}